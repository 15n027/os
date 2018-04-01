#include <stdio.h>

#include "basic_types.h"
#include "x86/exception.h"
#include "x86/dt.h"
#include "debug.h"
#include "x86/x86_defs.h"
#include "kernel.h"

void
idt_common(IntrFrame32 *frame)
{
    printf("INT/EXC VEC: %llu err: %#llx cs:ip %#llx:%#llx\n", frame->vector, frame->errcode,
            frame->cs, frame->eip);
    if (frame->vector == EXC_PF) {
        printf("#PF CR2=%x\n", GET_CR2());
    }
    if ((frame->cs & 3) != 0) {
        printf("CPL change detected: ss:esp %08llx:%08llx\n", frame->ss, frame->esp);
    }
    //    ASSERT(frame->vector == 4);
    ASSERT_ON_COMPILE(EXC_OF == 4);
    if (frame->vector == EXC_BP || frame->vector == EXC_OF) {
        return;
    }
    for(;;)
        asm("cli;hlt");
}
