#include <string.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "kassert.h"
#include "x86/cpuid.h"
#include "loader.h"
#include "multiboot/multiboot.h"
#include "pagealloc.h"
#include "kernel.h"
#include "basic_types.h"
#include "x86/x86_defs.h"
#include "i386/paging32.h"
#include "x86/dt.h"

void earlyconsole_init(void);

static void
init_pic(void)
{
    asm("outb %0, $0xa1\n"
        "outb %0, $0x21\n"
        :: "a"(0xff));
}

static void
init_phys_map(multiboot_info_t *mbi)
{
    multiboot_memory_map_t *mmap;

    ASSERT_ON_COMPILE(sizeof *mbi <= PAGE_SIZE);
    if (mbi->flags & MULTIBOOT_INFO_MEM_MAP) {
        mmap=(multiboot_memory_map_t*)(uintptr_t)mbi->mmap_addr;
        while ((uintptr_t)mmap + sizeof(*mmap) <= mbi->mmap_addr + mbi->mmap_length) {
            printf("[%08llx - %08llx ] RAM:%d(%d)\n",
                    mmap->addr, mmap->addr + mmap->len, (mmap->type == MULTIBOOT_MEMORY_AVAILABLE) ? 1 : 0, mmap->type);
            mark_phys_range(mmap->addr, mmap->len, mmap->type == MULTIBOOT_MEMORY_AVAILABLE);
            mmap = (multiboot_memory_map_t*)((uintptr_t)mmap + mmap->size + sizeof(mmap->size));
        }
    }
    mark_phys_range(PTR_TO_PA(mbi), sizeof *mbi, FALSE);
    if (mbi->flags & MULTIBOOT_INFO_MODS) {
        multiboot_module_t *mods = (void*)mbi->mods_addr;
        mark_phys_range(mbi->mods_addr, mbi->mods_count * sizeof *mods, FALSE);
        for (uint32 i = 0; i < mbi->mods_count; i++) {
            mark_phys_range(mods[i].cmdline, strlen((char*)mods[i].cmdline) + 1, FALSE);
            mark_phys_range(mods[i].mod_start, mods[i].mod_end - mods[i].mod_start, FALSE);
        }
    }
    mark_phys_range(0, PAGE_SIZE, FALSE);
    mark_phys_range(START_KERNEL, END_KERNEL - START_KERNEL, FALSE);
    mark_phys_range(0xb8000, PAGE_SIZE, FALSE);
    complete_phys_map();
    print_phys_map();
}


void kern_entry(uint32_t mbsig, multiboot_info_t *mbi)
{
    init_pic();
    earlyconsole_init();
    if (mbsig != MULTIBOOT_BOOTLOADER_MAGIC) {
        printf("32->64 loader wasn't loaded via multiboot.\n");
        HALT();
    }
    if (!cpuid_isset(LM) || !cpuid_isset(NX)) {
        printf("LM or NX are unsupported\n");
        HALT();
    }
    init_phys_map(mbi);
    printf("mbsig=0x%x mbi=%p\n", mbsig, mbi);
    printf("flags=0x%x mem_lower=0x%x mem_upper=0x%x boot=0x%x\n",
            mbi->flags, mbi->mem_lower, mbi->mem_upper, mbi->boot_device);
    printf("bootloadername=%s\n", (char*)mbi->boot_loader_name);
    load_gdt();
    load_idt();
    enter_mode_ia32e();
    asm volatile("int3");
    asm("pushf\n"
        "push %0\n"
        "popf\n"
        "into\n"
        "popf\n"
            : : "g"(EFLAGS_OF | 2));

    printf("survived interrupt\n");
    map_pages(PTR_TO_VA(mbi), PTR_TO_VA(mbi),
            PAGES_SPANNED(PTR_TO_VA(mbi), PTR_TO_VA(mbi) + sizeof *mbi), PT_P | PT_NX);
    if (mbi->flags & MULTIBOOT_INFO_MODS) {
        multiboot_module_t *mods = (void*)mbi->mods_addr;
        printf("MB mods count=%u addr=%p\n", mbi->mods_count, mods);
        map_pages(mbi->mods_addr, mbi->mods_addr,
                PAGES_SPANNED(mbi->mods_addr, mbi->mods_addr + mbi->mods_count * sizeof *mods),
                PT_NX | PT_P);
        for (uint32 i = 0; i < mbi->mods_count; i++) {
            uint64 entry;
            Regs64 handoff = {0};
            map_page(mods[i].cmdline, mods[i].cmdline, PT_P | PT_NX);
            map_pages(mods[i].mod_start, mods[i].mod_start,
                    PAGES_SPANNED(mods[i].mod_start, mods[i].mod_end), PT_P | PT_NX);
            entry = load_module(PA_TO_PTR(mods[i].cmdline), PA_TO_PTR(mods[i].mod_start),
                    mods[i].mod_end - mods[i].mod_start);
            ASSERT(entry != 0);
            handoff.rdi = mbsig;
            handoff.rsi = PTR_TO_VA(mbi);
            handoff.rip = entry;
            farjump_to_64(&handoff);
        }
    }
    HALT();
}
