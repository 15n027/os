#pragma once
#include "kassert.h"
#include "basic_types.h"
#include "basic_defs.h"


static inline bool
Atomic_TestAndSet32(uint32 *p, unsigned bit)
{
    uint8 wasSet;
    ASSERT(bit < 32);
    ASSERT((VA)p % sizeof *p == 0);
    asm("lock; bts %1, %2\n"
        "setc %0\n"
        : "=g"(wasSet)
        : "g" (bit), "m" (*p)
        : "cc", "memory");
    return wasSet;
}

static inline void
Atomic_ClearBit32(uint32 *p, unsigned bit)
{
    ASSERT(bit < 32);
    ASSERT((VA)p % sizeof *p == 0);
    asm("lock; and %0, %1\n"
        :
        : "g"(~(1u << bit)), "m" (*p)
        : "cc", "memory");
}

static inline void
Atomic_And32(uint32 *p, uint32 mask)
{
    ASSERT((VA)p % sizeof *p == 0);
    asm("lock; and %0, %1\n"
        :
        : "g"(mask), "m" (*p)
        : "cc", "memory");
}

#define Make_Atomic_Read(sz)                                \
    static inline uint##sz                                  \
    Atomic_Read##sz(uint##sz *p)                            \
    {                                                       \
        uint##sz ret;                                       \
        ASSERT((VA)p % sizeof *p == 0);                      \
        asm("mov %1, %0" : "=r"(ret) : "m"(*p) : "memory"); \
        return ret;                                         \
    }

Make_Atomic_Read(8)
Make_Atomic_Read(16)
Make_Atomic_Read(32)
Make_Atomic_Read(64)
#undef Make_Atomic_Read

#define Make_Atomic_Write(sz)                                           \
    static inline void                                                  \
    Atomic_Write##sz(uint##sz *p, uint##sz val)                         \
    {                                                                   \
        ASSERT((VA)p % sizeof *p == 0);                                 \
        asm("mov %0, %1" : "=rcK"(val) : "m"(*p) : "memory");           \
    }

Make_Atomic_Write(8)
Make_Atomic_Write(16)
Make_Atomic_Write(32)
Make_Atomic_Write(64)
#undef Make_Atomic_Write
