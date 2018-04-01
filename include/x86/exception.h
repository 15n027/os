#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "basic_types.h"

enum {
#define ENTRY(name, num, type, err) EXC_ ## name = num,
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
    uint32 eflags;
} Regs32;

typedef struct {
    uint64 rax;
    uint64 rcx;
    uint64 rdx;
    uint64 rbx;
    uint64 rsp;
    uint64 rbp;
    uint64 rsi;
    uint64 rdi;
    uint64 r8;
    uint64 r9;
    uint64 r10;
    uint64 r11;
    uint64 r12;
    uint64 r13;
    uint64 r14;
    uint64 r15;
    uint64 pad[2];
    uint64 rip;
    uint64 rflags;
    uint64 pad2[2];
} Regs64;

typedef struct {
    uint64 rax;
    uint64 rcx;
    uint64 rdx;
    uint64 rbx;
    uint64 rbp;
    uint64 rsi;
    uint64 rdi;
    uint64 r8;
    uint64 r9;
    uint64 r10;
    uint64 r11;
    uint64 r12;
    uint64 r13;
    uint64 r14;
    uint64 r15;
    uint64 vector;
    uint64 errcode;
    uint64 rip;
    uint64 cs;
    uint64 rflags;
    /* If privilege change */
    uint64 rsp;
    uint64 ss;
} IntrFrame64;

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
    uint64 ss;
    uint64 esp;
} IntrFrame32;

#endif
