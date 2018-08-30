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
#include "smp/smp.h"
#include "x86/cpuid.h"
#include "percpu.h"
#include "x86/msr.h"


void init_apic(void);

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
            Log("[%08llx - %08llx ] RAM:%d(%d)\n",
                mmap->addr, mmap->addr + mmap->len, (mmap->type == MULTIBOOT_MEMORY_AVAILABLE) ? 1 : 0, mmap->type);
            mmap = (multiboot_memory_map_t*)((uintptr_t)mmap + mmap->size + sizeof(mmap->size));
        }
    }
}

static spinlock console_lock;


void pic_init(void);
volatile int x[2];

void
kern_entry(uint32 mbsig, multiboot_info_t *mbi)
{
    // disable ps2 ports
    OUTB(0x64, 0xad);
    OUTB(0x64, 0xa7);
    earlyconsole_init();
    puts("made it to 64 bit mode woot");
    printf("WRFSGSBASE: %d\n", cpuid_isset(FSGSBASE));
    printf("has FSGS.Base MSR: %d\n", cpuid_isset(MSRFSGSBASE));
    DBG("WRMSR: %d", cpuid_isset(HASMSR));
    ASSERT(cpuid_isset(SSE3));
    WRMSR(IA32_MISC_ENABLE, RDMSR(IA32_MISC_ENABLE) | (1 << 18));
    printf("has mwait: %d\n", cpuid_isset(MWAIT));
    printf("IA32_MISC_ENABLE: 0x%016lx\n", RDMSR(IA32_MISC_ENABLE));
    pic_init();
    cpu_init();
    Log("mbsig=%x mbi=%p\n", mbsig, mbi);
    if (mbsig == MULTIBOOT_BOOTLOADER_MAGIC) {
        print_mmap(mbi);
        pmm_init_multiboot(mbi);
    }
    PerCpuInit(TRUE);
    vmm_init();
    init_apic();
    ENABLE_INTERRUPTS();
    InitAcpi();
    serial_lateinit();
    //    BootAP(1);
    Log("ints on, hlt\n");
    ENABLE_INTERRUPTS();
    for(;;) {
        x[0]++;
        continue;
        if (x[0] % 10240 == 0) {
            spin_lock(&console_lock);
            Log("2 %08x %08x\r", x[0], x[1]);
            spin_unlock(&console_lock);
        }
    }
    asm("int3\n");
    HALT();
}
