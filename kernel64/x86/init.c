#include "debug.h"
#include "kernel.h"
#include "x86/dt.h"
#include "x86/x86_defs.h"
#include "x86/msr.h"

extern void idt_init(void);

static Tss64 tss0;

static uint64 gdt[] ALIGNED(8) = {
    [0] = 0,
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
    /* TSS is 16 bytes */
    [7] = 0,
    [8] = 0,
};

static void
gdt_init(void)
{
    extern const char gdt_init_load_cs[];
    FarPtr64 target64;
    DTR64 gdtr;
    TssGdtEntry *tss;
    target64.cs = DT_KERN64_CODE_SEL;
    target64.ip = PTR_TO_VA(gdt_init_load_cs);
    gdtr.limit = sizeof(gdt) - 1;
    gdtr.base = PTR_TO_VA(&gdt[0]);

    ASSERT_ON_COMPILE(sizeof *tss == 16);
    tss = (TssGdtEntry*)&gdt[7];
    tss->limit_0_15 = sizeof(tss0) - 1;
    tss->base_0_15 = LOWORD((VA)&tss0);
    tss->base_16_23 = (VA)&tss0 >> 16;
    tss->p_dpl_type = 0x80 | TSS64_AVAIL;
    tss->g_avl_lim_16_19 = 0;
    tss->base_24_31 = (VA)&tss0 >> 24;
    tss->base_32_63 = HIDWORD((VA)&tss0);
    tss->reserved = 0;

    tss0.iopermBase = 0xffff;
    tss0.rsp0 = (VA)_stack0_top;
    asm volatile(
            "lgdtq %0\n"
            "mov %1, %%ds\n"
            "mov %1, %%ss\n"
            "mov %1, %%es\n"
            "mov %4, %%fs\n"
            "mov %4, %%gs\n"
            "rex.w; ljmp *%2\n"
            ".extern gdt_init_load_cs\n"
            "gdt_init_load_cs:\n"
            "ltr %w3\n"
            :
            : "m"(gdtr), "r"(DT_KERN_DATA_SEL), "m"(target64), "r"(7 << 3), "r"(0)
            : "memory");
}

void
cpu_init(void)
{
    gdt_init();
}
