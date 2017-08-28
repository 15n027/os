#pragma once

#include "basic_types.h"

#define IA32_APIC_BASE           0x1b
#define IA32_FEATURE_CONTROL     0x3a
#define IA32_SYSENTER_CS         0x174
#define IA32_SYSENTER_ESP        0x175
#define IA32_SYSENTER_EIP        0x176
#define IA32_MISC_ENABLE         0x1a0
#define IA32_EFER                0xc0000080
#define IA32_STAR                0xc0000081
#define IA32_LSTAR               0xc0000082
#define IA32_FMASK               0xc0000084
#define IA32_FS_BASE             0xc0000100
#define IA32_GS_BASE             0xc0000101
#define IA32_KERNEL_GS_BASE      0xc0000102


static inline uint64 RDMSR(uint32 msr)
{
    uint32 eax, edx;
    asm("rdmsr"
            : "=a"(eax), "=d"(edx)
            : "c"(msr));
    return QWORD(edx, eax);
}

static inline void WRMSR(uint32 msr, uint64 val)
{
    asm("wrmsr"
            :
            : "d"((uint32)HIDWORD(val)), "a"((uint32)LODWORD(val)), "c"(msr));
}

static inline uint64 GET_EFER(void)
{
    return RDMSR(IA32_EFER);
}

static inline void SET_EFER(uint64 val)
{
    WRMSR(IA32_EFER, val);
}
