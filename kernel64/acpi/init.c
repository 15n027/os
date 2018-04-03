#include "acpi.h"
#include <acpica/acpi.h>
#include "x86/x86_defs.h"
#include "basic_defs.h"

bool
init_acpi(void)
{
    ACPI_TABLE_MADT *apic;
    ACPI_SUBTABLE_HEADER *sub;
    ACPI_STATUS s;
    AcpiInitializeSubsystem();
    s = AcpiInitializeTables(NULL, 0, true);
    ASSERT(ACPI_SUCCESS(s));
    s = AcpiLoadTables();
    ASSERT(ACPI_SUCCESS(s));
    s = AcpiEnableSubsystem(ACPI_FULL_INITIALIZATION);
    printf("s=%x\n", s);
    ASSERT(ACPI_SUCCESS(s));
    s = AcpiInitializeObjects(ACPI_FULL_INITIALIZATION);
    ASSERT(ACPI_SUCCESS(s));
    s = AcpiGetTable(ACPI_SIG_MADT, 1, (ACPI_TABLE_HEADER**)&apic);
    ASSERT(ACPI_SUCCESS(s));

    printf("madt=%p addr=%x len=%u flags=%x\n", apic, apic->Address, apic->Header.Length, apic->Flags);

    sub = (void*)((uint8*)apic + sizeof *apic);
    VA limit = (VA)apic + apic->Header.Length;
    while ((VA)sub + sizeof *sub <= limit) {
        switch (sub->Type) {
        case ACPI_MADT_TYPE_LOCAL_APIC: {
            ACPI_MADT_LOCAL_APIC *la = (void*)sub;
            ASSERT((VA)la + sizeof *la <= limit);
            printf("ProcesorId=%u apicid=%u flags=%u\n", la->ProcessorId, la->Id, la->LapicFlags);
            break;
        }
        case ACPI_MADT_TYPE_IO_APIC: {
            ACPI_MADT_IO_APIC *ioapic = (void*)sub;
            printf("ioapic id=%u address=%x irqbase=%u\n", ioapic->Id, ioapic->Address, ioapic->GlobalIrqBase);
            break;
        }
        case ACPI_MADT_TYPE_INTERRUPT_OVERRIDE: {
            ACPI_MADT_INTERRUPT_OVERRIDE *o = (void*)sub;
            printf("int override: Bus=%u source=%u global=%u flags=%u\n", o->Bus, o->SourceIrq, o->GlobalIrq, o->IntiFlags);
            break;
        }
        case ACPI_MADT_TYPE_LOCAL_APIC_NMI: {
            ACPI_MADT_LOCAL_APIC_NMI *n = (void*)sub;
            printf("local nmi: procid=%u intiflags=%u lint=%u\n", n->ProcessorId, n->IntiFlags, n->Lint);
            break;
        }
        default:
            printf("type=%u\n", sub->Type);
            NOT_IMPLEMENTED();
        }
        sub = (void*)((uint8*)sub + sub->Length);
    }
    return true;
}
