#include "x86/x86_defs.h"
#include "x86/exception.sinc"
void
idt_common(IretFrame *frame, unsigned vector, unsigned errCode)
{
    DBG("idt_common: frame=%p vector=0x%x errCode=0x%x", frame, vector, errCode);
    if (vector == EXC_BP || vector == EXC_OF) {
        return;
    }
    DBG("Unhandled exception: 0x%x rip=0x%llx rsp=0x%llx", vector, frame->rip, frame->rsp);
    HALT();
}
