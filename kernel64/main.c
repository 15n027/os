#include <stdio.h>
#include "basic_types.h"
#include "kassert.h"
#include "debug.h"
#include "multiboot/multiboot.h"
#include "memmap.h"
#include "x86/x86_defs.h"
#include <stdlib.h>
#include <acpica/acpi.h>

void cpu_init(void);
void
earlyconsole_init(void);

static void
print_mmap(const multiboot_info_t *mbi)
{
    multiboot_memory_map_t *mmap;
    DBG("mbi=%p flags=%p %x %x", mbi, &mbi->flags, mbi->flags, MULTIBOOT_INFO_MEM_MAP);
    ASSERT_ON_COMPILE(sizeof *mbi <= PAGE_SIZE);
    if (mbi->flags & MULTIBOOT_INFO_MEM_MAP) {
        DBG("");
        mmap = PA_TO_PTR(mbi->mmap_addr);
        while ((uintptr_t)mmap + sizeof(*mmap) <= mbi->mmap_addr + mbi->mmap_length) {
            printf("[%08llx - %08llx ] RAM:%d(%d)\n",
                    mmap->addr, mmap->addr + mmap->len, (mmap->type == MULTIBOOT_MEMORY_AVAILABLE) ? 1 : 0, mmap->type);
            mmap = (multiboot_memory_map_t*)((uintptr_t)mmap + mmap->size + sizeof(mmap->size));
        }
    }
}

static void
init_acpi(void)
{
    ACPI_TABLE_MADT *apic, *srat;
    ACPI_STATUS s;
    AcpiInitializeSubsystem();
    s = AcpiInitializeTables(NULL, 0, true);
    ASSERT(ACPI_SUCCESS(s));
    s = AcpiLoadTables();
    ASSERT(ACPI_SUCCESS(s));
    s = AcpiEnableSubsystem(ACPI_FULL_INITIALIZATION);
    printf("s=%x\n", s);
    ASSERT(ACPI_SUCCESS(s));
    s = AcpiGetTable(ACPI_SIG_MADT, 1, (ACPI_TABLE_HEADER**)&apic);
    ASSERT(ACPI_SUCCESS(s));
    printf("madt=%p addr=%x len=%u\n", apic, apic->Address, apic->Header.Length);
    s = AcpiGetTable(ACPI_SIG_SRAT, 1, (ACPI_TABLE_HEADER**)&srat);
    ASSERT(ACPI_SUCCESS(s));
    printf("srat=%p len=%u\n", srat, srat->Header.Length);

}

void pic_init(void);

void
kern_entry(uint32 mbsig, multiboot_info_t *mbi)
{
    earlyconsole_init();
    puts("made it to 64 bit mode woot");
    pic_init();
    cpu_init();
    asm("sti");
    printf("mbsig=%x mbi=%p\n", mbsig, mbi);
    if (mbsig == MULTIBOOT_BOOTLOADER_MAGIC) {
        print_mmap(mbi);
        pmm_init_multiboot(mbi);
    }
    vmm_init();
    init_acpi();

    asm("int3\n");
    HALT();
}
