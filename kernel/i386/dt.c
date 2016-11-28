#include <stdint.h>
#include "util/ctor.h"

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) baselimit;

#define G_SHIFT (23 + 32)
#define D_SHIFT (22 + 32)
#define P_SHIFT (15 + 32)
#define LIMIT2_SHIFT (16 + 32)
#define LIMIT2_MASK 0xf000000000000ull
#define DPL_SHIFT (13 + 32)
#define TYPE_SHIFT (8 + 32)
#define FLAT_LIMIT 0xfffff

#define G (1ull << G_SHIFT)
#define D (1ull << D_SHIFT)
#define B D
#define P (1ull << P_SHIFT)
#define DPL(dpl) ((uint64_t)(dpl) << DPL_SHIFT)
#define LIMIT(limit) ((limit) & 0xffff) | (((uint64_t)(limit) << LIMIT2_SHIFT) & LIMIT2_MASK)
#define TYPE(type) ((uint64_t)(type) << TYPE_SHIFT)

enum DTYPE {
    CS_CONFORMING_READABLE_ACCESSED = 0x1f,
    CS_CONFORMING_READABLE_NOTACCESSED = 0x1e,
    CS_CONFORMING_NOTREADABLE_ACCESSED = 0x1d,
    CS_CONFORMING_NOTREADABLE_NOTACCESSED = 0x1c,
    CS_NONCONFORMING_READABLE_ACCESSED = 0x1b,
    CS_NONCONFORMING_READABLE_NOTACCESSED = 0x1a,
    CS_NONCONFORMING_NOTREADABLE_ACCESSED = 0x19,
    CS_NONCONFORMING_NOTREADABLE_NOTACCESSED = 0x18,

    DS_EXUP_WRITEABLE_ACCESSED = 0x13,
    DS_EXUP_WRITEABLE_NOTACCESSED = 0x12,
    DS_EXUP_NOTWRITEABLE_ACCESSED = 0x11,
    DS_EXUP_NOTWRITEABLE_NOTACCESSED = 0x10,
    DS_EXDOWN_WRITEABLE_ACCESSED = 0x17,
    DS_EXDOWN_WRITEABLE_NOTACCESSED = 0x16,
    DS_EXDOWN_NOTWRITEABLE_ACCESSED = 0x15,
    DS_EXDOWN_NOTWRITEABLE_NOTACCESSED = 0x14,

    TSS16_AVAIL = 1,
    LDT = 2,
    TSS16_BUSY = 3,
    CALLGATE16 = 4,
    TASKGATE = 5,
    INTGATE16 = 6,
    TRAPGATE16 = 7,
    TSS32_AVAIL = 9,
    TSS32_BUSY = 11,
    CALLGATE32 = 12,
    INTGATE32 = 14,
    TRAPGATE32 = 15,
};

void load_gdt(void)
{
    baselimit addr;
    static const uint64_t gdt[5] __attribute__((aligned(0x1000))) = {
        [1] = G | D | P | LIMIT(FLAT_LIMIT) |
              DPL(0) |
              TYPE(CS_CONFORMING_READABLE_ACCESSED),
        [2] = G | B | P | LIMIT(FLAT_LIMIT) |
              DPL(0) |
              TYPE(DS_EXUP_WRITEABLE_ACCESSED),
        [3] = G | D | P | LIMIT(FLAT_LIMIT) |
              DPL(3) |
              TYPE(CS_CONFORMING_READABLE_ACCESSED),
        [4] = G | B | P | LIMIT(FLAT_LIMIT) |
              DPL(3) |
              TYPE(DS_EXUP_WRITEABLE_ACCESSED)
    };

    addr.limit = sizeof(gdt[0]) * 5 - 1;
    addr.base = (uintptr_t)&gdt[0];
    asm volatile("lgdt %0\n"
            "mov %1, %%ds\n"
            "mov %1, %%ss\n"
            "mov %1, %%es\n"
            "mov %1, %%fs\n"
            "mov %1, %%gs\n"
            "ljmp %2, $1f\n"
            "1:\n"
            :
            : "m"(addr), "r"(0x10), "K"(0x08));
}

void load_idt(void)
{
#define OFFS(off) (((((uint64_t)(off)) << 32) & 0xffff000000000000ull) | ((off) & 0xffffull))
#define SEG(cs) ((cs) << 16)
extern const void *const idt_base;
#define IDTDESC(x) SEG(0x08) | OFFS((x) * 8 + (uintptr_t)&idt_base) | P | DPL(0) | TYPE(INTGATE32)
    baselimit idtr;
    int i;
    static uint64_t idt[256] __attribute__((aligned(0x1000)));

    for (i = 0; i < 256; i++) {
        idt[i] = IDTDESC(i);
    }
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = sizeof(idt) - 1;
    asm volatile ("lidt %0\n"
            :
            : "m" (idtr));
}

CTOR(load_idt);
CTOR(load_gdt);
