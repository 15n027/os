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

#define NOTHING()
#define PUSHZERO() "push $0\n"
#define ENTRY(name, num, type, err) ENTRY_##err(name, num)
#define ENTRY_ASM(name, num, maybepushzero)                             \
    ".globl idt_entry_" #name "\n"                                      \
    ".align 16\n"                                                       \
    "idt_entry_" #name ":\n"                                            \
    maybepushzero                                                       \
    "push $" STRINGIZE(num) "\n"                                        \
    "jmp idt_common_asm\n"

#define ENTRY_HASERR(name, num) ENTRY_ASM(name, num, NOTHING())
#define ENTRY_NOERR(name, num)  ENTRY_ASM(name, num, PUSHZERO())

asm(
#include "x86/idt-table.h"
);

#undef ENTRY
#undef ENTRY_HASERR
#undef ENTRY_NOERR
#undef PUSHZERO
#undef NOTHING

#define ENTRY(name, num, type, err) extern const char idt_entry_##name[];
#include "x86/idt-table.h"
#undef ENTRY

#define SEG(cs) ((cs) << 16)

typedef struct interrupt_handler_list {
    interrupt_handler fn;
    struct interrupt_handler_list *next;
} interrupt_handler_list;

static Gate idt[256];

static interrupt_handler_list *handlers[256];

static inline Gate
IDTDESC(uint64 handler)
{
    Gate ret;

    ret.hi = HIDWORD(handler);
    ret.lo = ((HIWORD(handler) << 48) | LOWORD(handler)) |
             DT_P | DT_DPL(0) | DT_TYPE(INTGATE64) | SEG(DT_KERN64_CODE_SEL) |
             DT_IST(0);
    return ret;
}

void
idt_init(void)
{
    BaseLimit64 idtr = {.base = PTR_TO_VA(&idt[0]), .limit = sizeof(idt) - 1};
    uint64 base = PTR_TO_VA(idt_entry_DE);
    uint32 i;
#define ENTRY(name, num, type, err)                         \
    ASSERT(PTR_TO_VA(idt_entry_##name) == base + num * 16); \
    ASSERT_ON_COMPILE(EXC_##name < 256);                    \
    ASSERT_ON_COMPILE(EXC_##name == num);
#include "x86/idt-table.h"
#undef ENTRY

    for (i = 0; i < 256; i++) {
        idt[i] = IDTDESC(base + i * 16);
    }
    asm volatile("lidt %0\n" : : "m"(idtr));
}

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

void
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
            return;
        }
        for (;;) {
            HALT();
        }
    }
}