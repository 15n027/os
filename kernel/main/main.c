#include <string.h>
#include <stdio.h>
#include "device.h"
#include "debug.h"
#include "startup.h"
#include "multiboot/multiboot.h"

SET_DEFINE(sys_startup, sys_startup_fn);

void earlyconsole_init(void);

static void
print_mmap(multiboot_info_t *mbi)
{
    multiboot_memory_map_t *mmap;
    if (mbi->flags & MULTIBOOT_INFO_MEM_MAP) {
        printf("mmap_len=%d addr=%x\n", mbi->mmap_length, mbi->mmap_addr);
        mmap=(multiboot_memory_map_t*)(uintptr_t)mbi->mmap_addr;
        while ((uintptr_t)mmap + sizeof(*mmap) < mbi->mmap_addr + mbi->mmap_length) {
            printf("addr: [ %08llx - %08llx ] RAM: %s\n",
                    mmap->addr, mmap->addr + mmap->len, mmap->type == 1 ? "YES" : "NO");
            mmap = (multiboot_memory_map_t*)((uintptr_t)mmap + mmap->size + sizeof(mmap->size));
        }
    }
}

extern const char _sctors[], _ectors[];

void kern_entry(uint32_t mbsig, multiboot_info_t *mbi)
{
    extern void load_idt(void);
    sys_startup_fn *sys_startup_func;
    uintptr_t ptr = (uintptr_t)_sctors, end = (uintptr_t)_ectors;
    earlyconsole_init();
    printf("ptr=%lx end=%lx diff=%lu\n", ptr, end, (end - ptr) / 4);
    while (ptr < end) {
        void (**fn)(void) = (void(**)(void))ptr;
        (*fn)();
        ptr += sizeof(fn);
    }
    SET_FOREACH(sys_startup, sys_startup_func) {
        printf("calling startup fn: %p\n", *sys_startup_func);
        (*sys_startup_func)();
    }
    //    load_gdt();
    //load_idt();
    printf("mbsig=0x%x mbi=%p\n", mbsig, mbi);
    printf("flags=0x%x mem_lower=0x%x mem_upper=0x%x boot=0x%x\n",
            mbi->flags, mbi->mem_lower, mbi->mem_upper, mbi->boot_device);
    print_mmap(mbi);
    printf("device_register\n");
    device_register();
    printf("ud2\n");
    asm volatile("mov $0x8888, %eax; mov %eax, %gs\n");
    printf("survived?\n");
    for(;;);
}
