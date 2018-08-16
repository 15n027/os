#pragma once
#include "basic_types.h"
#include "basic_defs.h"
#include "dt.h"
#include <limits.h>
#ifndef PAGE_SIZE
#error "PAGE_SIZE undefined by limits.h"
#endif

#define PAGE_MASK (PAGE_SIZE - 1)
#define PAGE_SHIFT 12
#define LPAGE_SIZE (0x200000)
#define LPAGE_MASK (LPAGE_SIZE - 1)
#define LPAGE_SHIFT 21

#define EFLAGS_CF (1 << 0)
#define EFLAGS_PF (1 << 2)
#define EFLAGS_AF (1 << 4)
#define EFLAGS_ZF (1 << 6)
#define EFLAGS_SF (1 << 7)
#define EFLAGS_TF (1 << 8)
#define EFLAGS_IF (1 << 9)
#define EFLAGS_DF (1 << 10)
#define EFLAGS_OF (1 << 11)
#define EFLAGS_IOPL_SHIFT 12
#define EFLAGS_NT (1 << 14)
#define EFLAGS_RF (1 << 16)
#define EFLAGS_VM (1 << 17)
#define EFLAGS_AC (1 << 18)
#define EFLAGS_VIF (1 << 19)
#define EFLAGS_VIP (1 << 20)
#define EFLAGS_ID (1 << 21)

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

typedef struct IretFrame {
    uint64 rip;
    uint64 cs;
    uint64 flags;
    uint64 rsp;
    uint64 ss;
} IretFrame;

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

static inline void SET_DS(uint16 sel)
{
    asm volatile ("mov %0, %%ds\n" :: "r" (sel));
}
static inline void SET_ES(uint16 sel)
{
    asm volatile ("mov %0, %%es\n" :: "r" (sel));
}
static inline void SET_SS(uint16 sel)
{
    asm volatile ("mov %0, %%ss\n" :: "r" (sel));
}
static inline void SET_FS(uint16 sel)
{
    asm volatile ("mov %0, %%fs\n" :: "r" (sel));
}
static inline void SET_GS(uint16 sel)
{
    asm volatile ("mov %0, %%gs\n" :: "r" (sel));
}

#define SET_CS(sel) asm volatile("ljmp %0, $1f;1:" :: "K"(sel))

static inline void GET_GDT32(DTR32 *dtr)
{
    asm ("sgdt %0" : "=m" (*dtr));
}

static inline void SET_GDT32(DTR32 dtr)
{
    asm ("lgdt %0" : : "m" (dtr));
}

static inline void GET_IDT32(DTR32 *dtr)
{
    asm ("sidt %0" : "=m" (*dtr));
}

static inline void SET_IDT32(DTR32 dtr)
{
    asm ("lidt %0" : : "m" (dtr));
}

static inline uint64 RDTSC(void)
{
    uint32 eax, edx;
    asm volatile ("rdtsc\n" : "=a"(eax), "=d"(edx) :: "memory");
    return QWORD(edx, eax);
}
static inline void PAUSE(void)
{
    asm("pause\n");
}

static inline uint8 INB(uint16 port)
{
    uint8 val;
    if (__builtin_constant_p(port) && port <= 0xff) {
        asm volatile("inb %1, %%al" : "=a"(val) : "N"(port) : "memory");
    } else {
        asm volatile("inb %%dx, %%al" : "=a"(val) : "d"(port) : "memory");
    }
    return val;
}

static inline uint16 INW(uint16 port)
{
    uint16 val;
    if (__builtin_constant_p(port) && port <= 0xff) {
        asm volatile("inw %1, %%ax" : "=a"(val) : "N"(port) : "memory");
    } else {
        asm volatile("inw %%dx, %%ax" : "=a"(val) : "d"(port) : "memory");
    }
    return val;
}

static inline uint32 IND(uint16 port)
{
    uint32 val;
    if (__builtin_constant_p(port) && port <= 0xff) {
        asm volatile("inl %1, %%eax" : "=a"(val) : "N"(port) : "memory");
    } else {
        asm volatile("inl %%dx, %%eax" : "=a"(val) : "d"(port) : "memory");
    }
    return val;
}

static inline void OUTB(uint16 port, uint8 val)
{
   if (__builtin_constant_p(port) && port <= 0xff) {
       asm volatile("outb %%al, %1" :: "a"(val), "N"(port) : "memory");
   } else {
       asm volatile("outb %%al, %%dx" :: "a"(val), "d"(port) : "memory");
   }
}

static inline void OUTW(uint16 port, uint16 val)
{
   if (__builtin_constant_p(port) && port <= 0xff) {
       asm volatile("outw %%ax, %1" :: "a"(val), "N"(port) : "memory");
   } else {
       asm volatile("outw %%ax, %%dx" :: "a"(val), "d"(port) : "memory");
   }
}

static inline void OUTD(uint16 port, uint32 val)
{
   if (__builtin_constant_p(port) && port <= 0xff) {
       asm volatile("outl %%eax, %1" :: "a"(val), "N"(port) : "memory");
   } else {
       asm volatile("outl %%eax, %%dx" :: "a"(val), "d"(port) : "memory");
   }
}

static inline void ENABLE_INTERRUPTS(void)
{
    asm volatile("sti" ::: "memory");
}

static inline void DISABLE_INTERRUPTS(void)
{
    asm volatile("cli" ::: "memory");
}

static inline bool INTERRUPTS_ENABLED(void)
{
    uintptr_t flags;
    asm volatile("pushf;"
                 "popf"
                 : "=g"(flags));
    return (flags & EFLAGS_IF) != 0;
}

static inline void MMIO_WRITE8(uint8 *x, uint8 val)
{
    asm ("movb %1, %0" :: "m"(*(x)), "rcK"(val) : "memory");
}
static inline void MMIO_WRITE16(uint16 *x, uint16 val)
{
    asm ("movw %1, %0" :: "m"(*(x)), "rcK"(val) : "memory");
}
static inline void MMIO_WRITE32(uint32 *x, uint32 val)
{
    asm ("movl %1, %0" :: "m"(*(x)), "rcK"(val) : "memory");
}
static inline void MMIO_WRITE64(uint64 *x, uint64 val)
{
    asm ("movq %1, %0" :: "m"(*(x)), "g"(val) : "memory");
}


static inline uint8 MMIO_READ8(uint8 *x)
{
    uint8 ret;
    asm ("movb %1, %0" : "=g"(ret) : "m"(*(x)) : "memory");
    return ret;
}

static inline uint16 MMIO_READ16(uint16 *x)
{
    uint8 ret;
    asm ("movw %1, %0" : "=g"(ret) : "m"(*(x)) : "memory");
    return ret;
}
static inline uint32 MMIO_READ32(uint32 *x)
{
    uint32 ret;
    asm ("movl %1, %0" : "=g"(ret) : "m"(*(x)) : "memory");
    return ret;
}
static inline uint64 MMIO_READ64(uint64 *x)
{
    uint64 ret;
    asm ("movq %1, %0" : "=g"(ret) : "m"(*(x)) : "memory");
    return ret;
}

static inline void MFENCE(void)
{
    asm("mfence" ::: "memory");
}
