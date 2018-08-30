#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include "x86/dt.h"
#include "x86/x86_defs.h"
#include "basic_defs.h"
#include "kassert.h"
#include "memmap.h"
#include "interrupt.h"
#include "x86/exception.sinc"

typedef struct interrupt_handler_list {
    interrupt_handler fn;
    struct interrupt_handler_list *next;
} interrupt_handler_list;


static interrupt_handler_list *handlers[256];

bool
install_handler(uint8 vector, interrupt_handler fn)
{
    interrupt_handler_list *newnode = malloc(sizeof *newnode);
    interrupt_handler_list **list;
    if (!newnode) {
        return false;
    }
    newnode->fn = fn;
    newnode->next = NULL;
    list = &handlers[vector];
    while (*list) {
        list = &(*list)->next;
    }
    *list = newnode;
    return true;
}

static void
dump_frame(IretFrame *frame)
{
    uint64 *stk = (void*)frame->rsp;
    printf("RSP:%016lx RIP:%02lx:%016lx\n"
           "STK: %016lx %016lx %016lx %016lx %016lx %016lx\n",
           frame->rsp, frame->cs, frame->rip,
           stk[0], stk[1], stk[2], stk[3], stk[4], stk[5]);
}

void
idt_common(IretFrame *frame, uint32 vector, uint32 errCode)
{
    interrupt_handler_list *handler;
    bool handled = false;
    ASSERT(vector < ARRAYSIZE(handlers));
    if (vector == EXC_PF) {
        handled = handle_pf(frame);
    }
    handler = handlers[vector];
    while (handler) {
        handled |= handler->fn(frame);
        handler = handler->next;
    }
    if (!handled) {
        if (vector == EXC_PF) {
            printf("UNHANDLED #PF: err: 0x%x cr2: 0x%lx cs:ip %02"PRIx64":%#"PRIx64"\n",
                   errCode, GET_CR2(), frame->cs, frame->rip);
        } else if (vector != EXC_BP && vector != EXC_OF) {
            printf("UNHANDLED INT/EXC: 0x%x err: 0x%x cs:ip %02"PRIx64":%#"PRIx64"\n",
                   vector, errCode, frame->cs, frame->rip);
        }
        if ((frame->cs & 3) != 0) {
            printf("CPL change detected: ss:esp %08"PRIx64":%08"PRIx64"\n", frame->ss,
                   frame->rsp);
        }
        if (vector == EXC_BP || vector == EXC_OF) {
            return;
        }
        dump_frame(frame);
        DBG("");
        for (;;) {
            HALT();
        }
    }
}
