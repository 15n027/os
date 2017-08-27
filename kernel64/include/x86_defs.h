#pragma once
#include "basic_defs.h"
#include <limits.h>
#ifndef PAGE_SIZE
#error "PAGE_SIZE undefined by limits.h"
#endif

#define PAGE_MASK (PAGE_SIZE - 1)
#define PAGE_SHIFT 12
#define LPAGE_SIZE (0x200000)
#define LPAGE_MASK (LPAGE_SIZE - 1)
#define LPAGE_SHIFT 21

#define EFER_SCE (1 << 0)
#define EFER_LME (1 << 8)
#define EFER_LMA (1 << 10)
#define EFER_NXE (1 << 11)

#define CR0_PE         (1 << 0)
#define CR0_MP         (1 << 1)
#define CR0_EM         (1 << 2)
#define CR0_TS         (1 << 3)
#define CR0_ET         (1 << 4)
#define CR0_NE         (1 << 5)
#define CR0_WP         (1 << 16)
#define CR0_AM         (1 << 18)
#define CR0_NW         (1 << 29)
#define CR0_CD         (1 << 30)
#define CR0_PG         (1 << 31)

#define CR4_VME        (1 << 0)
#define CR4_PVI        (1 << 1)
#define CR4_TSD        (1 << 2)
#define CR4_DE         (1 << 3)
#define CR4_PSE        (1 << 4)
#define CR4_PAE        (1 << 5)
#define CR4_MCE        (1 << 6)
#define CR4_PGE        (1 << 7)
#define CR4_PCE        (1 << 8)
#define CR4_OSFXSR     (1 << 9)
#define CR4_OSXMMEXCPT (1 << 10)
#define CR4_UMIP       (1 << 11)
#define CR4_VMXE       (1 << 13)
#define CR4_SMXE       (1 << 14)
#define CR4_FSGSBASE   (1 << 16)
#define CR4_PCIDE      (1 << 17)
#define CR4_OSXSAVE    (1 << 18)
#define CR4_SMEP       (1 << 20)
#define CR4_SMAP       (1 << 21)
#define CR4_PKE        (1 << 22)
#define CR4_TPL_MASK   0xf

#define SET_CR0(val) SET_CR(0, val)
#define SET_CR2(val) SET_CR(2, val)
#define SET_CR3(val) SET_CR(3, val)
#define SET_CR4(val) SET_CR(4, val)
#define SET_CR8(val) SET_CR(8, val)

#define GET_CR0(val) GET_CR(0)
#define GET_CR2(val) GET_CR(2)
#define GET_CR3(val) GET_CR(3)
#define GET_CR4(val) GET_CR(4)
#define GET_CR8(val) GET_CR(8)

static inline uintptr_t GET_CR(uint32 cr)
{
#define GET_CRX(cr, val) asm volatile("mov %%cr" STRINGIZE(cr) ", %0" : "=r"(val))
    uintptr_t ret;
    switch (cr) {
    case 0: GET_CRX(0, ret); break;
    case 2: GET_CRX(2, ret); break;
    case 3: GET_CRX(3, ret); break;
    case 4: GET_CRX(4, ret); break;
    case 8: GET_CRX(8, ret); break;
    default:
        NOT_REACHED();
    }
    return ret;
#undef GET_CRX
}

static inline void SET_CR(uint32 cr, uintptr_t val)
{
#define SET_CRX(cr, val) asm volatile("mov %0, %%cr" STRINGIZE(cr) : : "r"(val))
    switch (cr) {
    case 0: SET_CRX(0, val); break;
    case 2: SET_CRX(2, val); break;
    case 3: SET_CRX(3, val); break;
    case 4: SET_CRX(4, val); break;
    case 8: SET_CRX(8, val); break;
    default:
        NOT_REACHED();
    }
#undef SET_CRX
}
