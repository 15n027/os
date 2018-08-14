#include <acpica/acpi.h>

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "memmap.h"
#include "x86/paging.h"
#include "interrupt.h"

void AcpiOsPrintf(const char *fmt, ...)
{
    va_list val;
    va_start(val, fmt);
    vprintf(fmt, val);
    va_end(val);
}

void AcpiOsVprintf(const char *fmt, va_list val)
{
    vprintf(fmt, val);
}

void AcpiOsFree(void *v)
{
    free(v);
}

void *AcpiOsAllocate(ACPI_SIZE size)
{
    void *p = malloc(size);
    ASSERT(p != NULL);
    return p;
}

ACPI_STATUS AcpiOsInitialize(void)
{
    return AE_OK;
}

ACPI_STATUS AcpiOsTerminate(void)
{
    return AE_OK;
}

ACPI_PHYSICAL_ADDRESS AcpiOsGetRootPointer(void)
{
    ACPI_SIZE ptr;
    if (AcpiFindRootPointer(&ptr) == AE_OK) {
        return ptr;
    }
    ASSERT(!"failed to find rdsp");
    return 0;
}

ACPI_STATUS AcpiOsCreateLock(ACPI_SPINLOCK *lock)
{
    *lock = 0;
    return AE_OK;
}

ACPI_STATUS AcpiOsCreateSemaphore(UINT32 max, UINT32 initial, ACPI_SEMAPHORE *sema)
{
    *sema = 0;
    return AE_OK;
}

ACPI_CPU_FLAGS AcpiOsAcquireLock(ACPI_SPINLOCK lock)
{
    return 0;
}

void AcpiOsReleaseLock(ACPI_SPINLOCK lock, ACPI_CPU_FLAGS flags)
{
}

void AcpiOsDeleteLock(ACPI_SPINLOCK lock)
{
    NOT_IMPLEMENTED();
}

ACPI_STATUS AcpiOsDeleteSemaphore(ACPI_SEMAPHORE sema)
{
    NOT_IMPLEMENTED();
    return AE_OK;
}

ACPI_THREAD_ID AcpiOsGetThreadId(void)
{
    return 1;
}

ACPI_STATUS
AcpiOsPhysicalTableOverride (
        ACPI_TABLE_HEADER *ExistingTable,
        ACPI_PHYSICAL_ADDRESS *NewAddress,
        UINT32 *NewTableLength)
{
    *NewAddress = 0;
    return AE_OK;
}

ACPI_STATUS
AcpiOsWaitSemaphore (
        ACPI_SEMAPHORE Handle,
        UINT32 Units,
        UINT16 Timeout)
{
    return AE_OK;
}

ACPI_STATUS
AcpiOsSignalSemaphore (
        ACPI_SEMAPHORE Handle,
        UINT32 Units)
{
    return AE_OK;
}

UINT64 AcpiOsGetTimer(void)
{
    //    NOT_IMPLEMENTED();
    return 0;
}

ACPI_STATUS
AcpiOsReadPciConfiguration (
        ACPI_PCI_ID *PciId,
        UINT32 Register,
        UINT64 *Value,
        UINT32 Width)
{
    //    NOT_IMPLEMENTED();
    return AE_OK;
}

ACPI_STATUS
AcpiOsWritePciConfiguration (
    ACPI_PCI_ID             *PciId,
    UINT32                  Reg,
    UINT64                  Value,
    UINT32                  Width)
{
    //    NOT_IMPLEMENTED();
    return AE_OK;
}

ACPI_STATUS
AcpiOsSignal (
    UINT32                  Function,
    void                    *Info)
{
    NOT_IMPLEMENTED();
    return AE_OK;
}

void *
AcpiOsMapMemory (
    ACPI_PHYSICAL_ADDRESS   Where,
    ACPI_SIZE               Length)
{
    PA pa = ROUNDDOWN(Where, PAGE_SIZE);
    ACPI_SIZE len = PAGES_SPANNED(Where, Where + Length);
    VA va = alloc_va_from(get_kern_vma(), VM_AREA_ACPI, len);
    ASSERT(va != 0);
    map_pages(pa, va, len, PT_NX);
    //    printf("%s: %lx (%lx) -> %lx Length=0x%lx len=0x%lx\n", __func__, Where, pa, va, Length, len);
    va |= (Where & PAGE_MASK);
    return (void*)va;
}
void
AcpiOsUnmapMemory (
    void                    *LogicalAddress,
    ACPI_SIZE               Size)
{
    //    VA va = ROUNDDOWN((VA)LogicalAddress, PAGE_SIZE);
    //  ACPI_SIZE len = PAGES_SPANNED((VA)LogicalAddress, (VA)LogicalAddress + Size);
    // printf("%s: %p 0x%lx\n", __func__, LogicalAddress, Size);
    //    unmap_pages(va, len);
}

ACPI_STATUS
AcpiOsTableOverride (
    ACPI_TABLE_HEADER       *ExistingTable,
    ACPI_TABLE_HEADER       **NewTable)
{
    *NewTable = NULL;
    return AE_OK;
}



void
AcpiOsWaitEventsComplete (
    void)
{
    NOT_IMPLEMENTED();
}

void
AcpiOsSleep (
    UINT64                  Milliseconds)
{
    NOT_IMPLEMENTED();
}

void
AcpiOsStall (
    UINT32                  Microseconds)
{
    uint64 tsc = RDTSC();
    while (RDTSC() - tsc < Microseconds * 1000 * 4) PAUSE();
}


ACPI_STATUS
AcpiOsExecute (
    ACPI_EXECUTE_TYPE       Type,
    ACPI_OSD_EXEC_CALLBACK  Function,
    void                    *Context)
{
    NOT_IMPLEMENTED();
    return AE_OK;
}

ACPI_STATUS
AcpiOsReadPort (
    ACPI_IO_ADDRESS         Address,
    UINT32                  *Value,
    UINT32                  Width)
{
    switch (Width) {
    case 8:
        *Value = INB(Address);
        break;
    case 16:
        *Value = INW(Address);
        break;
    case 32:
        *Value = IND(Address);
        break;
    default:
        NOT_IMPLEMENTED();
    }
    return AE_OK;
}

ACPI_STATUS
AcpiOsWritePort (
    ACPI_IO_ADDRESS         Address,
    UINT32                  Value,
    UINT32                  Width)
{
    switch (Width) {
    case 8:
        OUTB(Address, Value);
        break;
    case 16:
        OUTW(Address, Value);
        break;
    case 32:
        OUTD(Address, Value);
        break;
    default:
        NOT_IMPLEMENTED();
    }
    return AE_OK;
}

ACPI_STATUS
AcpiOsReadMemory (
    ACPI_PHYSICAL_ADDRESS   Address,
    UINT64                  *Value,
    UINT32                  Width)
{
    NOT_IMPLEMENTED();
    return AE_OK;
}

ACPI_STATUS
AcpiOsWriteMemory (
    ACPI_PHYSICAL_ADDRESS   Address,
    UINT64                  Value,
    UINT32                  Width)
{
    NOT_IMPLEMENTED();
    return AE_OK;
}

static bool
acpi_handler(IretFrame *frame)
{
    printf("%s\n", __func__);
    return true;
}

ACPI_STATUS
AcpiOsInstallInterruptHandler (
    UINT32                  InterruptNumber,
    ACPI_OSD_HANDLER        ServiceRoutine,
    void                    *Context)
{
    install_handler(InterruptNumber + 32, acpi_handler);
    printf("%s: %u %p %p\n", __func__, InterruptNumber, ServiceRoutine, Context);
    return AE_OK;
}

ACPI_STATUS
AcpiOsRemoveInterruptHandler (
    UINT32                  InterruptNumber,
    ACPI_OSD_HANDLER        ServiceRoutine)
{
    NOT_IMPLEMENTED();
    return AE_OK;
}

ACPI_STATUS
AcpiOsPredefinedOverride (
    const ACPI_PREDEFINED_NAMES *InitVal,
    ACPI_STRING                 *NewVal)
{
    *NewVal = NULL;
    return AE_OK;
}
