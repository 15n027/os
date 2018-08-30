#include <stdlib.h>
#include <acpica/acpi.h>
#include "x86/x86_defs.h"
#include "x86/paging.h"
#include "memmap.h"
#include "x86/cpuid.h"
#include "x86/msr.h"
#include "interrupt.h"
#include "apic.h"

static bool x2;
static uint8 *APIC;

void ApicEOI(void)
{
    ApicWrite(APIC_EOI, 0);
}

void
ApicWrite(uint32 reg, uint64 val)
{
    if (x2) {
        WRMSR(0x800 | (reg >> 4), val);
    } else {
        MMIO_WRITE32((uint32*)(APIC + reg), val);
    }
}

uint64
ApicRead(uint32 reg)
{
    uint64 val;
    if (x2) {
        val = RDMSR(0x800 | (reg >> 4));
    } else if (APIC) {
        val = MMIO_READ32((uint32*)(APIC + reg));
    } else {
        return INVALID_X2APIC_ID;
    }
    return val;
}

void
ApicIPI(uint32 apicId, uint32 flags)
{
    if (x2) {
        uint64 val = QWORD(apicId, flags);
        ApicWrite(APIC_ICRLO, val);
    } else {
        ASSERT(apicId < INVALID_APIC_ID);
        ApicWrite(APIC_ICRHI, apicId << 24);
        ApicWrite(APIC_ICRLO, flags);
    }
}

static bool
handler_22(IretFrame *unused, void *ctx)
{
    asm("" ::: "memory");
    ApicEOI();
    return true;
}

static bool
handler_21(IretFrame *unused, void *ctx)
{
    DBG("%s", __func__);
    ApicEOI();
    return true;
}

void
init_apic(void)
{
    VA apicBase = RDMSR(IA32_APIC_BASE);

    install_handler(0x22, handler_22, NULL);
    install_handler(0x21, handler_21, NULL);
    if (cpuid_isset(X2APIC)) {
        printf("X2APIC supported\n");
        apicBase |= 3 << 10;
        x2 = true;
    } else {
        APIC = (void*)alloc_va(NULL, 1);
        printf("Using xAPIC at %p\n", APIC);
        map_page(apicBase & ~PAGE_MASK, (VA)APIC, PT_RW | PT_P | PT_NX);
        apicBase |= (1 << 11);
    }
    printf("IA32_APIC_BASE=0x%016lx\n", apicBase);
    WRMSR(IA32_APIC_BASE, apicBase);
    ApicWrite(APIC_INITIAL_COUNT, 0);
    DBG("APIC_LVT_TIMER=%016lx", ApicRead(APIC_LVT_TIMER));
    ApicWrite(APIC_SPURIOUS, 0x1ff);
    for (unsigned i = APIC_LVT_TIMER; i <= APIC_LVT_ERR; i += 0x10) {
        DBG("[%03x] %04lx", i, ApicRead(i));
        ApicWrite(i, 1 << 16);
    }
}

uint32
MyApicId(void)
{
    uint32 id = ApicRead(APIC_ID);
    if (!x2) {
        id >>= 24;
    }
    return id;
}
