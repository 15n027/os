#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <stdint.h>

enum {
    EXC_DE = 0,
    EXC_DB,
    EXC_NMI,
    EXC_BP,
    EXC_OF,
    EXC_BR = 5,
    EXC_UD,
    EXC_NM,
    EXC_DF = 8,
    EXC_TS = 10,
    EXC_NP,
    EXC_SS,
    EXC_GP,
    EXC_PF,
    EXC_MF = 16,
    EXC_AC,
    EXC_MC,
    EXC_XF,
};

typedef struct {
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    uint32_t flags;
} Regs32;

typedef struct {
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t edx;
    uint32_t ecx;
    uint32_t ebx;
    uint32_t eax;
    uint32_t vector_idx;
    uint32_t errcode;
    uint32_t eip;
    uint32_t cs;
    uint32_t flags;
    /* If privilege change */
    uint32_t esp;
    uint32_t ss;
} IntrFrame;

#endif
