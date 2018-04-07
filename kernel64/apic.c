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

inline void
apic_write(uint32 reg, uint64 val)
{
    if (x2) {
        WRMSR(0x800 | (reg >> 4), val);
    } else {
        MMIO_WRITE32((uint32*)(APIC + reg), val);
    }
}

inline uint64
apic_read(uint32 reg)
{
    if (x2) {
        return RDMSR(0x800 | (reg >> 4));
    } else {
        return MMIO_READ32((uint32*)(APIC + reg));
    }
    return 0;
}

static bool
handler_22(IntrFrame64 *frame)
{
    //    printf("TICK\n");
    apic_write(APIC_EOI, 0);
    return true;
}

static bool
handler_21(IntrFrame64 *frame)
{
    DBG("%s", __func__);
    apic_write(APIC_EOI, 0x21);
    return true;
}

void
init_apic(void)
{
    VA apicBase = RDMSR(IA32_APIC_BASE);

    install_handler(0x22, handler_22);
    install_handler(0x21, handler_21);
    if (cpuid_isset(X2APIC)) {
        printf("X2APIC supported\n");
        apicBase |= 3 << 10;
        x2 = true;
    } else {
        APIC = (void*)alloc_va(get_kern_vma(), 1);
        printf("Using xAPIC at %p\n", APIC);
        map_page(apicBase & ~PAGE_MASK, (VA)APIC, PT_RW | PT_P | PT_NX);
        apicBase |= (1 << 11);
    }
    printf("IA32_APIC_BASE=0x%016lx\n", apicBase);
    WRMSR(IA32_APIC_BASE, apicBase);
    apic_write(APIC_INITIAL_COUNT, 0);
    DBG("APIC_LVT_TIMER=%016lx", apic_read(APIC_LVT_TIMER));
    apic_write(APIC_SPURIOUS, 0x1ff);
}
