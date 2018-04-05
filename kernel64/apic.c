#include <stdlib.h>
#include <acpica/acpi.h>
#include "x86/x86_defs.h"
#include "x86/paging.h"
#include "memmap.h"
#include "x86/cpuid.h"
#include "x86/msr.h"
#include "interrupt.h"

enum {
    APIC_ID  = 0x20,
    APIC_VER = 0x30,
    APIC_TPR = 0x80,
    APIC_APR = 0x90,
    APIC_PPR = 0xA0,
    APIC_EOI = 0xB0,
    APIC_RRD = 0xC0,
    APIC_LDR = 0xD0,
    APIC_DFR = 0xE0,
    APIC_SPURIOUS = 0xF0,
    APIC_ISR0 = 0x100,
    APIC_ISR1 = 0x110,
    APIC_ISR2 = 0x120,
    APIC_ISR3 = 0x130,
    APIC_ISR4 = 0x140,
    APIC_ISR5 = 0x150,
    APIC_ISR6 = 0x160,
    APIC_ISR7 = 0x170,
    APIC_TMR0 = 0x180,
    APIC_TMR1 = 0x190,
    APIC_TMR2 = 0x1A0,
    APIC_TMR3 = 0x1B0,
    APIC_TMR4 = 0x1C0,
    APIC_TMR5 = 0x1D0,
    APIC_TMR6 = 0x1E0,
    APIC_TMR7 = 0x1F0,
    APIC_IRR0 = 0x200,
    APIC_IRR1 = 0x210,
    APIC_IRR2 = 0x220,
    APIC_IRR3 = 0x230,
    APIC_IRR4 = 0x240,
    APIC_IRR5 = 0x250,
    APIC_IRR6 = 0x260,
    APIC_IRR7 = 0x270,
    APIC_ESR = 0x280,
    APIC_LVT_CMCI = 0x2F0,
    APIC_ICRLO = 0x300,
    APIC_ICRHI = 0x310,
    APIC_LVT_TIMER = 0x320,
    APIC_LVT_THERMAL = 0x330,
    APIC_LVT_PMC = 0x340,
    APIC_LVT_LINT0 = 0x350,
    APIC_LVT_LINT1 = 0x360,
    APIC_LVT_ERR = 0x370,
    APIC_INITIAL_COUNT = 0x380,
    APIC_CURRENT_COUNT = 0x390,
    APIC_TIMER_DIVIDE = 0x3E0,
};

static bool x2;
static void
apic_write(uint32 reg, uint64 val)
{
    if (x2) {
        WRMSR(0x800 | (reg >> 4), val);
    } else {
        NOT_IMPLEMENTED();
    }
}

static uint64
apic_read(uint32 reg)
{
    if (x2) {
        return RDMSR(0x800 | (reg >> 4));
    } else {
        NOT_IMPLEMENTED();
    }
}

static bool
handler_22(IntrFrame64 *frame)
{
    printf("TICK\n");
    apic_write(APIC_EOI, 0);
    return true;
}

static bool
handler_21(IntrFrame64 *frame)
{
    DBG("");
    apic_write(APIC_EOI, 0);
    return true;
}

void
init_apic(void)
{

    install_handler(0x22, handler_22);
    install_handler(0x21, handler_21);
    if (cpuid_isset(X2APIC)) {
        uint64 val;
        printf("X2APIC supported\n");
        val = RDMSR(IA32_APIC_BASE);
        printf("IA32_APIC_BASE=0x%016lx\n", val);
        val |= 3 << 10;
        WRMSR(IA32_APIC_BASE, val);
        x2 = true;
        apic_write(APIC_INITIAL_COUNT, 0);
        DBG("APIC_LVT_TIMER=%016lx", apic_read(APIC_LVT_TIMER));
    }
    ENABLE_INTERRUPTS();
}
