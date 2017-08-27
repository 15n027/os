#include <stdint.h>
#include "debug.h"
#include "exception.h"
#include "dt.h"
#include "paging.h"
#include "x86_defs.h"
#include "kernel.h"
#include "msr.h"

static uint64 gdt[] = {
    [1] = DT_G | DT_L | DT_P | DT_DPL(0) |
          DT_TYPE(CS_NONCONFORMING_READABLE_NOTACCESSED) | DT_LIMIT(0xfffff),
    [2] = DT_G | DT_D | DT_P | DT_DPL(0) | DT_LIMIT(0xfffff) |
          DT_TYPE(DS_EXUP_WRITEABLE_NOTACCESSED),
    [3] = DT_G | DT_D | DT_P | DT_DPL(0) | DT_LIMIT(0xfffff) |
          DT_TYPE(CS_NONCONFORMING_READABLE_NOTACCESSED),
};

void
load_gdt(void)
{
    baselimit addr;
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
            : "m"(addr), "r"(DT_KERN_DATA_SEL), "K"(DT_KERN32_CODE_SEL)
            : "eax");
}


extern void init_idt_entries(Gate idt[static 256]);
void
load_idt(void)
{
    baselimit idtr;
    static Gate idt[256] __attribute__((aligned(0x1000)));

    init_idt_entries(idt);
    idtr.base = PTR_TO_VA(idt);
    idtr.limit = sizeof(idt) - 1;
    asm volatile ("lidt %0\n"
            :
            : "m" (idtr));
}

void
enter_mode_ia32e(void)
{
    init_4level_pagetable();
    SET_EFER(GET_EFER() | EFER_NXE | EFER_LME | EFER_LMA);
    enable_paging(get_paging_root());
}

void farjump_to_64(uint64 entry)
{
    extern const char in64Bit[];
    FarPtr32 fptr = {.cs = DT_KERN64_CODE_SEL, .ip = PTR_TO_VA(in64Bit)};
    asm volatile(
        "ljmp *%0\n"
        ".globl in64Bit\n"
        "in64Bit:\n"
        ".code64\n"
        "jmp *%1\n"
        ".code32\n"
            : : "m"(fptr), "m"(entry));
    NOT_REACHED();
}
