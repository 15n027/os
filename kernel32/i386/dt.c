#include <stdint.h>
#include "debug.h"
#include "startup.h"
#include "exception.h"
#include "dt.h"

void load_gdt(void)
{
    baselimit addr;
    static uint64_t gdt[] __attribute__((aligned(0x1000))) = {
        [1] = G | D | P | LIMIT(FLAT_LIMIT) |
              DPL(0) |
              TYPE(CS_NONCONFORMING_READABLE_ACCESSED),
        [2] = G | B | P | LIMIT(FLAT_LIMIT) |
              DPL(0) |
              TYPE(DS_EXUP_WRITEABLE_ACCESSED),
        [3] = G | D | P | LIMIT(FLAT_LIMIT) |
              DPL(3) |
              TYPE(CS_NONCONFORMING_READABLE_ACCESSED),
        [4] = G | B | P | LIMIT(FLAT_LIMIT) |
              DPL(3) |
              TYPE(DS_EXUP_WRITEABLE_ACCESSED)
    };
    DBG("");
    addr.limit = sizeof(gdt) - 1;
    addr.base = (uintptr_t)&gdt[0];
    asm volatile("lgdt %0\n"
            "mov %1, %%ds\n"
            "mov %1, %%ss\n"
            "mov %1, %%es\n"
            "xor %%eax, %%eax\n"
            "mov %%eax, %%fs\n"
            "mov %%eax, %%gs\n"
            "ljmp %2, $1f\n"
            "1:\n"
            :
            : "m"(addr), "r"(0x10), "K"(0x08)
            : "eax");
}


extern void init_idt_entries(uint64_t idt[256]);
void load_idt(void)
{
    baselimit idtr;
    static uint64_t idt[256] __attribute__((aligned(0x1000)));

    init_idt_entries(idt);
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = sizeof(idt) - 1;
    asm volatile ("lidt %0\n"
            :
            : "m" (idtr));
}

TEXT_SET(sys_startup, load_gdt);
TEXT_SET(sys_startup, load_idt);

