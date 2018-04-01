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
