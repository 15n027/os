#pragma once

#include "exception.h"
#include "basic_types.h"
#include "basic_defs.h"

#pragma pack(push, 1)

typedef struct {
    uint16 limit;
    uint32 base;
} DTR32;

typedef struct {
    uint16 limit;
    uint64 base;
} DTR64;

typedef struct {
    uint64 ip;
    uint16 cs;
} FarPtr64;

typedef struct {
    uint32 ip;
    uint16 cs;
} FarPtr32;

typedef struct {
    uint16 ip;
    uint16 cs;
} FarPtr16;

#pragma pack(pop)

typedef struct {
    uint64 lo;
    uint64 hi;
} Gate64;

#pragma pack(push, 1)
typedef struct {
    uint32 pad0;
    uint64 rsp0;
    uint64 rsp1;
    uint64 rsp2;
    uint64 pad1;
    uint64 ist[7];
    uint64 pad2;
    uint32 pad3;
    uint16 iopermBase;
} Tss64;

typedef struct {
    uint16 limit_0_15;
    uint16 base_0_15;
    uint8 base_16_23;
    uint8 p_dpl_type;
    uint8 g_avl_lim_16_19;
    uint8 base_24_31;
    uint32 base_32_63;
    uint32 reserved;
} TssGdtEntry;
#pragma pack(pop)

#define DT_G_SHIFT (23 + 32)
#define DT_D_SHIFT (22 + 32)
#define DT_L_SHIFT (21 + 32)
#define DT_P_SHIFT (15 + 32)
#define DT_DPL_SHIFT (13 + 32)
#define DT_TYPE_SHIFT (8 + 32)
#define DT_IST_SHIFT 32
#define DT_LIMIT(lim) QWORD(((lim) >> 16) << 16, (lim) & 0xffff)
#define DT_G (1ull << DT_G_SHIFT)
#define DT_D (1ull << DT_D_SHIFT)
#define DT_L (1ull << DT_L_SHIFT)
#define DT_B DT_D
#define DT_P (1ull << DT_P_SHIFT)
#define DT_DPL(dpl) ((uint64_t)(dpl) << DT_DPL_SHIFT)
#define DT_TYPE(type) ((uint64_t)(type) << DT_TYPE_SHIFT)
#define DT_IST(val) (((uint64)(val)) << DT_IST_SHIFT)
#define IDT_TARGET(x) QWORD(HIWORD(x) << 16, LOWORD(x))
enum DT_TYPE {
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
    TSS64_AVAIL = TSS32_AVAIL,
    TSS32_BUSY = 11,
    TSS64_BUSY = TSS32_BUSY,
    CALLGATE32 = 12,
    INTGATE32 = 14,
    INTGATE64 = INTGATE32,
    TRAPGATE32 = 15,
};

#define DT_CODE_SEL(bits, dpl) (DT_IDX_CS##bits##_DPL##dpl * 8 + dpl)
#define DT_DATA_SEL(dpl)       (DT_IDX_DS_DPL##dpl * 8 + dpl)
#define DT_KERN64_CODE_SEL     DT_CODE_SEL(64, 0)
#define DT_KERN32_CODE_SEL     DT_CODE_SEL(32, 0)
#define DT_KERN_DATA_SEL       DT_DATA_SEL(0)
#define DT_IDX_CS64_DPL0 1
#define DT_IDX_DS_DPL0   2
#define DT_IDX_CS32_DPL0 3

void farjump_to_64(const Regs64 *regs);
void load_idt(void);
void load_gdt(void);
void enter_mode_ia32e(void);
