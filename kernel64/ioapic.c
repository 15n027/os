#include <stdlib.h>
#include <acpica/acpi.h>
#include "x86/x86_defs.h"
#include "x86/paging.h"
#include "memmap.h"
#include "ioapic.h"


typedef struct {
    uint8 *IOREGSEL;
    uint32 *IOWIN;
    uint32 id;
    uint32 irqbase;
} IOApic;

static IOApic *ioApics;
static uint32 numIOApics;

void
IOApicWrite(IOApic *ioapic, uint32 reg, uint64 val)
{
    MMIO_WRITE8(ioapic->IOREGSEL, reg);
    MMIO_WRITE32(ioapic->IOWIN, LODWORD(val));
    if (reg >= IOAPIC_REDTBL(0) && reg < IOAPIC_REDTBL(IOAPIC_REDTBL_MAX)) {
        ASSERT(reg % 2 == 0);
        MMIO_WRITE8(ioapic->IOREGSEL, reg + 1);
        MMIO_WRITE32(ioapic->IOWIN, HIDWORD(val));
    }
}

uint64
IOApicRead(IOApic *ioapic, uint32 reg)
{
    uint32 hi = 0, lo;
    MMIO_WRITE8(ioapic->IOREGSEL, reg);
    lo = MMIO_READ32(ioapic->IOWIN);
    if (reg >= IOAPIC_REDTBL(0) && reg < IOAPIC_REDTBL(IOAPIC_REDTBL_MAX)) {
        ASSERT(reg % 2 == 0);
        MMIO_WRITE8(ioapic->IOREGSEL, reg + 1);
        hi = MMIO_READ32(ioapic->IOWIN);
    }
    return QWORD(hi, lo);
}

bool
RegisterIOApic(PA addr, const IOApicPin pins[static IOAPIC_REDTBL_MAX])
{
    IOApic *io, *new;
    void *map;

    numIOApics++;
    io = realloc(ioApics, numIOApics * sizeof *ioApics);
    PanicIf(!io, "IOAPIC alloc fail");
    ioApics = io;
    new = &ioApics[numIOApics - 1];
    map = (void*)alloc_va(NULL, 1);
    PanicIf(!map, "IOAPIC failed to map");
    map_page(addr, (VA)map, PT_RW | PT_NX | PT_P);

    new->IOREGSEL = (void*)map;
    new->IOWIN = (void*)(map + 0x10);
    new->id = 0;
    new->irqbase = 0;
    for (uint32 i = 0; i < IOAPIC_REDTBL_MAX; i++) {
        // physical dest mode, dest field = apic id 0, delivery mode fixed
        uint32 val = (pins[i].irq + 32) | pins[i].trigger | pins[i].polarity;
        IOApicWrite(new, IOAPIC_REDTBL(i), val);
    }
    return true;
}
