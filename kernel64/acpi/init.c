#include <stdlib.h>
#include "acpi.h"
#include <acpica/acpi.h>
#include "x86/x86_defs.h"
#include "basic_defs.h"
#include "smp/smp.h"
#include "x86/apic.h"
#include "ioapic.h"
#include "memmap.h"

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
ParseMadt(Madt *data)
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
        case ACPI_MADT_TYPE_LOCAL_X2APIC:
            sz = sizeof *data->x2apics;
            cursor = (void**)&data->x2apics;
            cnt = &data->numx2apics;
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


static void
InitIOApics(const Madt *madt)
{
    PA physAddr;
    uint32 i, j;

    for (i = 0; i < madt->numioapics; i++) {
        ACPI_MADT_IO_APIC *io = &madt->ioapics[i];
        IOApicPin pins[IOAPIC_REDTBL_MAX];
        physAddr = io->Address;

        for (j = 0; j < IOAPIC_REDTBL_MAX; j++) {
            pins[j].trigger = IOAPIC_TRIGGER_EDGE;
            pins[j].polarity = IOAPIC_POLARITY_ACTIVE_HI;
            pins[j].irq = j;
            for (uint32 k = 0; k < madt->numoverrides; k++) {
                ACPI_MADT_INTERRUPT_OVERRIDE *o = &madt->overrides[k];
                if (o->Bus == 0 && o->SourceIrq == j) {
                    DBG("override %u -> %u", j, o->GlobalIrq);
                    pins[j].polarity = (o->IntiFlags & 3) == 3 ? IOAPIC_POLARITY_ACTIVE_LO : IOAPIC_POLARITY_ACTIVE_HI;
                    pins[j].trigger = (o->IntiFlags & 0xc) == 0xc ? IOAPIC_TRIGGER_LEVEL : IOAPIC_TRIGGER_EDGE;
                    pins[j].irq = o->GlobalIrq;
                    break;
                }
            }
        }
        RegisterIOApic(physAddr, pins);
    }
}


static void
InitCpuToApicMapping(const Madt *madt)
{
    uint32 numCpus = 0;
    uint32 i;
    uint32 myId = MyApicId();

    for (i = 0; i < madt->numapics; i++) {
        numCpus = MAX(madt->apics[i].ProcessorId, numCpus);
    }
    for (i = 0; i < madt->numx2apics; i++) {
        numCpus = MAX(madt->x2apics[i].Uid, numCpus);
    }
    numCpus++;
    cpuToApicMap = malloc(sizeof *cpuToApicMap * numCpus);
    PanicIf(!cpuToApicMap, "Alloc cpuToApicMap fail");
    for (i = 0; i < numCpus; i++) {
        cpuToApicMap[i] = INVALID_X2APIC_ID;
    }
    for (i = 0; i < madt->numapics; i++) {
        cpuToApicMap[madt->apics[i].ProcessorId] = madt->apics[i].Id;
        if (myId != madt->apics[i].Id && (madt->apics[i].LapicFlags & 1)) {
            BootAP(madt->apics[i].Id);
        }
    }
    for (i = 0; i < madt->numx2apics; i++) {
        cpuToApicMap[madt->x2apics[i].Uid] = madt->x2apics[i].LocalApicId;
        if (myId != madt->x2apics[i].LocalApicId && (madt->x2apics[i].LapicFlags & 1)) {
            BootAP(madt->x2apics[i].LocalApicId);
        }
    }
}

static void
ParseHpet(void)
{
    ACPI_TABLE_HPET *hpet;
    PA addr;
    ACPI_STATUS s;
    uint64 *va;
    s = AcpiGetTable(ACPI_SIG_HPET, 1, (ACPI_TABLE_HEADER**)&hpet);
    ASSERT(ACPI_SUCCESS(s));
    addr = hpet->Address.Address;
    DBG("addr=%lx", addr);
    map_page(addr, addr, PT_RW | PT_P);
    va = (uint64*)addr;
    DBG("va[2] = %lx", va[2]);

    /* Set legacy routing and disable intrs.  For some reason with legacy routing off intrs still fire in qemu */
    va[2] = 2;
}

static ACPI_STATUS
AcpiGPEHandler(ACPI_HANDLE device, UINT32 gpeNum, void *ctx)
{
    DBG("");
    return AE_OK;
}

static UINT32
pwr_handler(void *ctx)
{
    ACPI_STATUS s;
    UINT32 status;
    s = AcpiGetEventStatus(ACPI_EVENT_POWER_BUTTON, &status);
    ASSERT(ACPI_SUCCESS(s));
    DBG("status = %x", status);
    ASSERT(status & ACPI_EVENT_FLAG_ENABLED);
    ASSERT(status & ACPI_EVENT_FLAG_HAS_HANDLER);
    ASSERT(status & ACPI_EVENT_FLAG_SET);
    AcpiClearEvent(ACPI_EVENT_POWER_BUTTON);
    DBG("ctx=%p", ctx);
    return 0;
}

bool
InitAcpi(void)
{
    ACPI_STATUS s;
    Madt madt = {0};
    AcpiInitializeSubsystem();
    s = AcpiInitializeTables(NULL, 0, true);
    ASSERT(ACPI_SUCCESS(s));
    s = AcpiLoadTables();
    ASSERT(ACPI_SUCCESS(s));
    s = AcpiEnableSubsystem(ACPI_FULL_INITIALIZATION);
    printf("s=%x\n", s);
    ASSERT(ACPI_SUCCESS(s));
    s = AcpiUpdateAllGpes();
    ASSERT(ACPI_SUCCESS(s));
    s = AcpiInstallGpeHandler(NULL, 0, ACPI_GPE_LEVEL_TRIGGERED, AcpiGPEHandler, NULL);
    ASSERT(ACPI_SUCCESS(s));
    s = AcpiInitializeObjects(ACPI_FULL_INITIALIZATION);
    ASSERT(ACPI_SUCCESS(s));
    s = AcpiEnable();
    ASSERT(ACPI_SUCCESS(s));
    //    s = AcpiEnterSleepStatePrep(2);
    //ASSERT(ACPI_SUCCESS(s));
    //AcpiEnterSleepState(5);

    s = AcpiEnableEvent(ACPI_EVENT_POWER_BUTTON, 0);
    ASSERT(ACPI_SUCCESS(s));
    s = AcpiInstallFixedEventHandler(ACPI_EVENT_POWER_BUTTON, pwr_handler, NULL);
    ASSERT(ACPI_SUCCESS(s));
    //    s = AcpiEnableEvent(ACPI_EVENT_GLOBAL, 0);

    ASSERT(ACPI_SUCCESS(s));
    s = AcpiEnableEvent(ACPI_EVENT_SLEEP_BUTTON, 0);
    ASSERT(ACPI_SUCCESS(s));
    if (ParseMadt(&madt)) {
        InitCpuToApicMapping(&madt);
        InitIOApics(&madt);
        free(madt.apics);
        free(madt.x2apics);
        free(madt.ioapics);
        free(madt.overrides);
    } else {
        VERIFY(0);
    }
    ParseHpet();
    return true;
}
