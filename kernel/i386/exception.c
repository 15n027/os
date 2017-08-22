#include "exception.h"
#include <stdio.h>
#include "dt.h"
#include "debug.h"

void idt_common(IntrFrame *frame)
{
    printf("INT/EXC VEC: %d err: %#x cs:ip %#x:%#x\n", frame->vector_idx, frame->errcode,
            frame->cs, frame->eip);
    if (frame->cs & 3) {
        printf("CPL change detected: ss:esp %08x:%08x\n", frame->ss, frame->esp);
    }

    for(;;)
        asm("cli;hlt");
}
