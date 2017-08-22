#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <stdint.h>

enum {
#define ENTRY(x, y) EXC_ ## x,
#include "idt-table.h"
#undef ENTRY
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
