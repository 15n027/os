#pragma once
#include "basic_types.h"

static inline unsigned
Atomic_Read32(unsigned *var)
{
    unsigned ret;
    asm ("mov %1, %0"
         : "=r" (ret)
         : "m" (*var));
    return ret;
}

static inline void
Atomic_Write32(unsigned *var, unsigned val)
{
    asm ("mov %0, %1"
         : "=m" (*var)
         : "ri" (val));
}

static inline void
Atomic_Write64(uint64 *var, uint64 val)
{
    asm ("movq %1, %0"
         : "=m" (*var)
         : "ri" (val)
         : "memory");
}

static inline uint64
Atomic_Read64(uint64 *var)
{
    uint64 ret;
    asm ("movq %1, %0"
         : "=r" (ret)
         : "m" (*var)
         : "memory");
    return ret;
}

static inline uint32
Atomic_CMPXCHG(uint32 *var, uint32 old, uint32 new)
{
    asm ("lock; cmpxchg %1, %0\n"
         : "+a" (old),
           "+m" (*var)
         : "r" (new)
         : "cc", "memory");
    return old;
}

static inline uint32
Atomic_ReadOr32(uint32 *var, uint32 val)
{
    uint32 old, new, result;
    do {
        old = *var;
        new = *var | val;
        result = Atomic_CMPXCHG(var, old, new);
    } while (result != old);
    return result;
}

static inline bool
Atomic_TestAndSet(uint8 *bits, uint32 idx)
{
    bool ret = 0;
    asm("lock; bts %1, %2\n"
        "setc %0\n"
        : "=g" (ret)
        : "ri" (idx),
          "m" (bits)
        : "cc", "memory");
    return ret;
}

static inline uint32
Atomic_ReadInc32(uint32 *val)
{
    uint32 ret = 1;
    asm ("lock; xadd %1, %0"
         : "+m" (*val),
           "+r" (ret)
         :
         : "cc", "memory");
    return ret;
}

static inline uint32
Atomic_ReadDec32(uint32 *val)
{
    uint32 ret = -1;
    asm ("lock; xadd %1, %0"
         : "+m" (val),
           "+r" (ret)
         :
         : "cc", "memory");
    return ret;
}
