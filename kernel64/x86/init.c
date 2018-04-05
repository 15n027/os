#include "debug.h"
#include "kernel.h"
#include "x86/exception.h"
#include "x86/dt.h"
#include "x86/x86_defs.h"
#include "x86/msr.h"

extern void idt_init(void);

static uint64 gdt[] ALIGNED(8) = {
    [1] = DT_G | DT_L | DT_P | DT_DPL(0) |
          DT_TYPE(CS_NONCONFORMING_READABLE_NOTACCESSED) | DT_LIMIT(0xfffff),
    [2] = DT_G | DT_D | DT_P | DT_DPL(0) | DT_LIMIT(0xfffff) |
          DT_TYPE(DS_EXUP_WRITEABLE_NOTACCESSED),
    [3] = DT_G | DT_D | DT_P | DT_DPL(0) | DT_LIMIT(0xfffff) |
          DT_TYPE(CS_NONCONFORMING_READABLE_NOTACCESSED),
    [4] = DT_G | DT_L | DT_P | DT_DPL(3) |
          DT_TYPE(CS_NONCONFORMING_READABLE_NOTACCESSED) | DT_LIMIT(0xfffff),
    [5] = DT_G | DT_D | DT_P | DT_DPL(3) | DT_LIMIT(0xfffff) |
          DT_TYPE(DS_EXUP_WRITEABLE_NOTACCESSED),
    [6] = DT_G | DT_D | DT_P | DT_DPL(3) | DT_LIMIT(0xfffff) |
          DT_TYPE(CS_NONCONFORMING_READABLE_NOTACCESSED),
};

static void
gdt_init(void)
{
    extern const char gdt_init_load_cs[];
    FarPtr64 target64;
    BaseLimit64 gdtr;

    target64.cs = DT_KERN64_CODE_SEL;
    target64.ip = PTR_TO_VA(gdt_init_load_cs);
    gdtr.limit = sizeof(gdt) - 1;
    gdtr.base = PTR_TO_VA(&gdt[0]);
    asm volatile(
            "lgdtq %0\n"
            "mov %1, %%ds\n"
            "mov %1, %%ss\n"
            "mov %1, %%es\n"
            "xor %%eax, %%eax\n"
            "mov %%eax, %%fs\n"
            "mov %%eax, %%gs\n"
            "rex.w; ljmp *%2\n"
            ".extern gdt_init_load_cs\n"
            "gdt_init_load_cs:\n"
            :
            : "m"(gdtr), "r"(DT_KERN_DATA_SEL), "m"(target64)
            : "eax");
}

void
cpu_init(void)
{
    gdt_init();
    idt_init();
    //paging_init();
}