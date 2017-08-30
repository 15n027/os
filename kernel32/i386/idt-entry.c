#include "x86/exception.h"
#include "x86/dt.h"
#include "basic_defs.h"
#include "kassert.h"

#define NOTHING()
#define PUSHZERO() "push $0\n"
#define ENTRY(name, num, type, err) ENTRY_##err(name, num)
#define ENTRY_ASM(name, num, maybepushzero)                             \
    ".globl idt_entry_" #name "\n"                                      \
    ".align 16\n"                                                       \
    "idt_entry_" #name ":\n"                                            \
    maybepushzero                                                       \
    "push $" STRINGIZE(num) "\n"                                        \
    "jmp idt_common_asm\n"

#define ENTRY_HASERR(name, num) ENTRY_ASM(name, num, NOTHING())
#define ENTRY_NOERR(name, num)  ENTRY_ASM(name, num, PUSHZERO())

asm(".code64\n"
#include "x86/idt-table.h"
    ".code32\n");
#undef ENTRY
#undef ENTRY_HASERR
#undef ENTRY_NOERR
#undef PUSHZERO
#undef NOTHING

#define ENTRY(name, num, type, err) extern const char idt_entry_##name[];
#include "x86/idt-table.h"
#undef ENTRY

#define SEG(cs) ((cs) << 16)

static inline Gate
IDTDESC(uint64 handler)
{
    Gate ret;

    ret.hi = HIDWORD(handler);
    ret.lo = ((HIWORD(handler) << 48) | LOWORD(handler)) |
             DT_P | DT_DPL(0) | DT_TYPE(INTGATE64) | SEG(DT_KERN64_CODE_SEL) |
             DT_IST(0);
    return ret;
}

void init_idt_entries(Gate idt[static 256])
{
    uint64 base = PTR_TO_VA(idt_entry_DE);
    uint32 i;
#define ENTRY(name, num, type, err)                         \
    ASSERT(PTR_TO_VA(idt_entry_##name) == base + num * 16); \
    ASSERT_ON_COMPILE(EXC_##name < 256);                    \
    ASSERT_ON_COMPILE(EXC_##name == num);
#include "x86/idt-table.h"
#undef ENTRY

    for (i = 0; i < 256; i++) {
        idt[i] = IDTDESC(base + i * 16);
    }
}

