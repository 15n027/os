#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "basic_types.h"

enum {
#define ENTRY(x, y) EXC_ ## x,
#include "idt-table.h"
#undef ENTRY
};

typedef struct {
    uint32 eax;
    uint32 ecx;
    uint32 edx;
    uint32 ebx;
    uint32 esp;
    uint32 ebp;
    uint32 esi;
    uint32 edi;
    uint32 flags;
} Regs32;

typedef struct {
    uint32 edi;
    uint32 esi;
    uint32 ebp;
    uint32 edx;
    uint32 ecx;
    uint32 ebx;
    uint32 eax;
    uint64 vector;
    uint64 errcode;
    uint64 eip;
    uint64 cs;
    uint64 eflags;
    /* If privilege change */
    uint64 esp;
    uint64 ss;
} IntrFrame;

#endif
