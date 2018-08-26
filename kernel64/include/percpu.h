#pragma once
#include "basic_types.h"

typedef struct PerCpu {
    VA scratchStart;
    uint32 scratchUsed;
} PerCpu;

static inline PerCpu *
GetPerCpu(void)
{
    PerCpu *ret;
    asm("mov %%gs:0, %0\n"
        : "=r" (ret));
    return ret;
}

void PerCpuInit(bool isBSP);
