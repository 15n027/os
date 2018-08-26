#pragma once

#include "basic_types.h"
#include "msrnum.sinc"

static inline uint64 RDMSR(uint32 msr)
{
    uint32 eax, edx;
    asm ("rdmsr" : "=a"(eax), "=d"(edx) : "c"(msr) : "memory");
    return QWORD(edx, eax);
}

static inline void WRMSR(uint32 msr, uint64 val)
{
    uint32 a = LODWORD(val);
    uint32 d = HIDWORD(val);
    asm ("wrmsr" :: "a"(a), "d"(d), "c"(msr) : "memory");
}

static inline uint64 GET_EFER(void)
{
    return RDMSR(IA32_EFER);
}

static inline void SET_EFER(uint64 val)
{
    WRMSR(IA32_EFER, val);
}
