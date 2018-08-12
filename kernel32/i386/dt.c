#include <stdint.h>
#include "debug.h"
#include "x86/dt.h"
#include "i386/paging32.h"
#include "x86/x86_defs.h"
#include "kernel.h"
#include "x86/msr.h"

extern const Gate64 IDT[];

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
    DTR32 addr;
    DBG("");
    addr.limit = sizeof(gdt) - 1;
    addr.base = (uintptr_t)&gdt[0];
    SET_GDT32(addr);
    SET_DS(DT_KERN_DATA_SEL);
    SET_ES(DT_KERN_DATA_SEL);
    SET_SS(DT_KERN_DATA_SEL);
    SET_DS(DT_KERN_DATA_SEL);
    SET_FS(0);
    SET_GS(0);
    SET_CS(DT_KERN32_CODE_SEL);
}

void
load_idt(void)
{
    DTR32 idtr;

    idtr.base = (VA)IDT;
    idtr.limit = 256 * sizeof IDT[0] - 1;
    SET_IDT32(idtr);
}

void
enter_mode_ia32e(void)
{
    init_4level_pagetable();
    SET_EFER(GET_EFER() | EFER_NXE | EFER_LME | EFER_LMA | EFER_SCE);
    enable_paging(get_paging_root());
}

uint64 farjump_to_64_target;
Regs64 farjump_to_64_ctx;
__attribute__((noreturn)) void farjump_to_64(const Regs64 *regs)
{
    extern const char tramp64[];
    DTR32 null_idtr = {0};
    FarPtr32 fptr = {.cs = DT_KERN64_CODE_SEL, .ip = PTR_TO_PA(tramp64)};
    farjump_to_64_ctx = *regs;
    farjump_to_64_target = regs->rip;
    asm volatile(
        "lidt %0\n"
        "ljmp *%1\n"
        ".code64\n"
        ".globl tramp64\n"
        "tramp64:\n"
        "pushq 0x88(%%rdi)\n"
        "popf\n"
        "mov 0x00(%%rdi), %%rax\n"
        "mov 0x08(%%rdi), %%rcx\n"
        "mov 0x10(%%rdi), %%rdx\n"
        "mov 0x18(%%rdi), %%rbx\n"
        "mov 0x30(%%rdi), %%rsi\n"
        "mov 0x40(%%rdi), %%r8\n"
        "mov 0x48(%%rdi), %%r9\n"
        "mov 0x50(%%rdi), %%r10\n"
        "mov 0x58(%%rdi), %%r11\n"
        "mov 0x60(%%rdi), %%r12\n"
        "mov 0x68(%%rdi), %%r13\n"
        "mov 0x70(%%rdi), %%r14\n"
        "mov 0x78(%%rdi), %%r15\n"
        "mov 0x38(%%rdi), %%rdi\n"
        "jmp *farjump_to_64_target(%%rip)\n"
        ".code32\n"
        :
        : "m" (null_idtr), "m"(fptr), "D"(regs));
    __builtin_unreachable();
}
