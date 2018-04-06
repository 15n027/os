#include <stdio.h>
#include "basic_types.h"
#include "kassert.h"
#include "debug.h"
#include "multiboot/multiboot.h"
#include "memmap.h"
#include "x86/x86_defs.h"
#include <stdlib.h>
#include <acpica/acpi.h>
#include "acpi.h"
#include "serial.h"

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


void pic_init(void);

void
kern_entry(uint32 mbsig, multiboot_info_t *mbi)
{
    earlyconsole_init();
    puts("made it to 64 bit mode woot");
    pic_init();
    cpu_init();
    printf("mbsig=%x mbi=%p\n", mbsig, mbi);
    if (mbsig == MULTIBOOT_BOOTLOADER_MAGIC) {
        print_mmap(mbi);
        pmm_init_multiboot(mbi);
    }
    vmm_init();
    init_acpi();
    void init_ioapic(void);
    init_ioapic();
    void init_apic(void);
    init_apic();

    ENABLE_INTERRUPTS();
    for(;;) asm("hlt");
    asm("int3\n");
    HALT();
}
