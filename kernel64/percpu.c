#include <string.h>
#include "percpu.h"
#include "x86/apic.h"
#include "x86/msr.h"
#include "memmap.h"

void
PerCpuInit(bool isBSP)
{
    static PerCpu perCpu0;
    static PerCpu *pPerCpu = &perCpu0;
    PerCpu *perCpu;
    uint32 offset;
    if (isBSP) {

        perCpu = &perCpu0;
        offset = 0;
    } else {
        NOT_IMPLEMENTED();
    }
    WRMSR(IA32_GS_BASE, (uintptr_t)&pPerCpu);
    // TODO - Use cpu number
    perCpu->scratchStart = SCRATCH_START + offset;
    perCpu->scratchUsed = 0;
    DBG("scratchstart=%lx", perCpu->scratchStart);
    for (unsigned i = 0; i < 512; i++) {
        map_page(0, perCpu->scratchStart + PAGE_SIZE * i, 0);
    }
    DBG("");
}
