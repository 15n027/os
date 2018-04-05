#include <stdlib.h>
#include <acpica/acpi.h>
#include "x86/x86_defs.h"
#include "x86/paging.h"
#include "memmap.h"

#define MAX_IOAPICS 8

#define IOAPICID 0
#define IOAPICVER 1
#define IOAPICARB 2
#define IOREDTBL(x) (0x10 + (x) * 2)
#define IOREDTBL_MAX (24)

typedef struct {
    uint8 *IOREGSEL;
    uint32 *IOWIN;
    uint32 id;
    uint32 irqbase;
} IOApic;

static IOApic *ioapics;
static uint32 numioapics;

typedef struct {
    uint32 apicId;
    uint32 procId;
    uint32 flags;
} apic;

typedef struct {
    ACPI_MADT_LOCAL_APIC *apics;
    uint32 numapics;
    ACPI_MADT_LOCAL_X2APIC *x2apics;
    uint32 numx2apics;
    ACPI_MADT_IO_APIC *ioapics;
    uint32 numioapics;
    ACPI_MADT_INTERRUPT_OVERRIDE *overrides;
    uint32 numoverrides;
    uint32 flags;
} Madt;

static bool
parse_madt(Madt *data)
{
    ACPI_TABLE_MADT *apic;
    ACPI_SUBTABLE_HEADER *sub;
    ACPI_STATUS s;
    void **cursor;
    uint32 *cnt;
    size_t sz;
    VA limit;

    s = AcpiGetTable(ACPI_SIG_MADT, 1, (ACPI_TABLE_HEADER**)&apic);
    if (!ACPI_SUCCESS(s)) {
        return false;
    }
    limit = (VA)apic + apic->Header.Length;
    printf("madt=%p addr=%x len=%u flags=%x\n", apic, apic->Address, apic->Header.Length, apic->Flags);
    data->flags = apic->Flags;

    sub = (void*)((uint8*)apic + sizeof *apic);
    while ((VA)sub + sizeof *sub <= limit) {
        switch (sub->Type) {
        case ACPI_MADT_TYPE_LOCAL_APIC:
            sz = sizeof *data->apics;
            cursor = (void**)&data->apics;
            cnt = &data->numapics;
            break;
        case ACPI_MADT_TYPE_IO_APIC:
            sz = sizeof *data->ioapics;
            cursor = (void**)&data->ioapics;
            cnt = &data->numioapics;
            break;
        case ACPI_MADT_TYPE_INTERRUPT_OVERRIDE:
            sz = sizeof *data->overrides;
            cursor = (void**)&data->overrides;
            cnt = &data->numoverrides;
            break;
        default:
            sz = 0;
            cursor = NULL;
            cnt = NULL;
            break;
        }
        if (cursor && (VA)sub + sub->Length <= limit) {
            size_t newsize = (*cnt + 1) + sz;
            void *alloc = realloc(*cursor, (*cnt + 1) * sz);
            if (!alloc) {
                printf("MADT parsing failed, out of memory in alloc of %lu bytes\n", newsize);
                return false;
            }
            *cursor = alloc;
            memcpy((uint8*)*cursor + *cnt * sz, sub, sz);
            ++*cnt;
        }
        sub = (void*)((uint8*)sub + sub->Length);
    }
    return true;
}


void init_ioapic(void)
{
    Madt madt = {0};
    uint32 i, j;
    parse_madt(&madt);

        /*
    int override: Bus=0 source=0 global=2 flags=0

IOAPIC APICID=16777216
IOAPIC APICVER=170011
[ 0] = 0000000000010000 [ 1] = 0000000000010000
[ 4] = 0000000000010000 [ 5] = 0000000000010000
[ 8] = 0000000000010000 [ 9] = 0000000000010000
[12] = 0000000000010000 [13] = 0000000000010000
[16] = 0000000000010000 [17] = 0000000000010000
[20] = 0000000000010000 [21] = 0000000000010000
        */

    ioapics = malloc(madt.numioapics * sizeof *ioapics);
    numioapics = madt.numioapics;
    ASSERT(ioapics);
    for (i = 0; i < madt.numioapics; i++) {
        ACPI_MADT_IO_APIC *io = &madt.ioapics[i];
        VA va = alloc_va(get_kern_vma(), 1);
        map_page(io->Address, va, PT_RW | PT_NX);
        ioapics[i].id = io->Id;
        ioapics[i].irqbase = io->GlobalIrqBase;
        ioapics[i].IOREGSEL = (uint8*)va;
        ioapics[i].IOWIN = (uint32*)(va + 0x10);
    }

    for (i = 0; i < numioapics; i++) {
        uint32 tbl[IOREDTBL_MAX];
        uint8 pic_to_apic[IOREDTBL_MAX];
        for (j = 0; j < IOREDTBL_MAX; j++) {
            uint32 polarity = 0;
            uint32 trigger = 0;
            pic_to_apic[j] = j;
            for (uint32 k = 0; k < madt.numoverrides; k++) {
                ACPI_MADT_INTERRUPT_OVERRIDE *o = &madt.overrides[k];
                if (o->Bus == 0 && o->SourceIrq == j) {
                    DBG("overrde %u -> %u", j, o->GlobalIrq);
                    polarity = (o->IntiFlags & 3) == 3 ? (1 << !3) : 0;
                    trigger = (o->IntiFlags & 0xc) == 0xc ? (1 << 15) : 0;
                    pic_to_apic[j] = o->GlobalIrq;
                    break;
                }
            }
            tbl[j] = polarity | trigger;
        }
        for (j = 0; j < IOREDTBL_MAX; j++) {
            const uint32 IRQ_OFFSET = 32;
            uint8 *sel = ioapics[i].IOREGSEL;
            uint32 *reg = ioapics[i].IOWIN;
            uint32 irq = j;
            uint32 disable = 0;
            uint32 data;
            if (pic_to_apic[j] == pic_to_apic[0]) {
                disable = 1 << 16;
            }
            data =  tbl[j] | disable | (irq + IRQ_OFFSET);
            MMIO_WRITE8(sel, IOREDTBL(j));
            MMIO_WRITE32(reg, data);
            MMIO_WRITE8(sel, IOREDTBL(j) + 1);
            MMIO_WRITE32(reg, 0);
        }
    }
    free(madt.apics);
    free(madt.x2apics);
    free(madt.overrides);
}
