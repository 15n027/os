#include "acpi.h"
#include <acpica/acpi.h>
#include "x86/x86_defs.h"
#include "basic_defs.h"

bool
init_acpi(void)
{
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
    return true;
}
