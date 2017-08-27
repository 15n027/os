#include "exception.h"
#include <stdio.h>
#include "basic_types.h"
#include "dt.h"
#include "debug.h"
#include "x86_defs.h"
#include "kernel.h"
#include "paging.h"

void
idt_common(IntrFrame *frame)
{
    printf("INT/EXC VEC: %llu err: %#llx cs:ip %#llx:%#llx\n", frame->vector, frame->errcode,
            frame->cs, frame->eip);
    if (frame->vector == EXC_PF) {
        printf("#PF CR2=%x\n", GET_CR2());
    }
    if ((frame->cs & 3) != 0) {
        printf("CPL change detected: ss:esp %08llx:%08llx\n", frame->ss, frame->esp);
    }

    for(;;)
        asm("cli;hlt");
}
