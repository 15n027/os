#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include "x86/exception.h"
#include "x86/dt.h"
#include "x86/x86_defs.h"
#include "basic_defs.h"
#include "kassert.h"
#include "memmap.h"
#include "interrupt.h"
#include "kern_sched.h"


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
dump_frame(IntrFrame64 *frame)
{
    uint64 *stk = (void*)frame->rsp;
    printf("RSP:%016lx RIP:%02lx:%016lx RAX:%016lx\n"
           "RBX:%016lx RCX:%016lx RDX:%016lx\n"
           "STK: %016lx %016lx %016lx %016lx\n",
           frame->rsp, frame->cs, frame->rip, frame->rax,
           frame->rbx, frame->rcx, frame->rdx,
           stk[-4], stk[-3], stk[-2], stk[-1]);
}

IntrFrame64 *
idt_common(IntrFrame64 *frame)
{
    interrupt_handler_list *handler;
    bool handled = false;
    ASSERT(frame->vector < ARRAYSIZE(handlers));
    if (frame->vector == EXC_PF) {
        handled = handle_pf(frame);
    }
    handler = handlers[frame->vector];
    while (handler) {
        handled |= handler->fn(frame);
        handler = handler->next;
    }
    if (!handled) {
        if (frame->vector == EXC_PF) {
            printf("UNHANDLED #PF: err: 0x%"PRIx64" cr2: %lx cs:ip %"PRIx64":%#"PRIx64"\n",
                   frame->errcode, GET_CR2(), frame->cs, frame->rip);
        } else if (frame->vector != EXC_BP && frame->vector != EXC_OF) {
            printf("UNHANDLED INT/EXC: 0x%"PRIx64" err: 0x%"PRIx64" cs:ip %"PRIx64":%#"PRIx64"\n",
                   frame->vector, frame->errcode, frame->cs, frame->rip);
        }
        if ((frame->cs & 3) != 0) {
            printf("CPL change detected: ss:esp %08"PRIx64":%08"PRIx64"\n", frame->ss,
                   frame->rsp);
        }
        if (frame->vector == EXC_BP || frame->vector == EXC_OF) {
            goto out;
        }
        dump_frame(frame);
        for (;;) {
            HALT();
        }
    }
out:
    ASSERT(frame);
    ASSERT(current);
    ASSERT(current->next);
    ASSERT(current->next->frame);
    //printf("current:%s next:%s\n", current->name, current->next->name);
    current->frame = frame;
    if (current != current->next) {
        Thread *next = current->next;
        //        asm volatile("xchgw %%bx, %%bx" ::: "rbx", "memory");
        //        printf("current!=next\ncs=%lx rip=%lx\n", next->frame->cs, next->frame->rip);
        current = next;
        return next->frame;
    }
    return current->next->frame;
}
