#pragma once

static inline unsigned Atomic_Read32(unsigned *var)
{
    unsigned ret;
    asm ("mov %1, %0"
         : "=r" (ret)
         : "m" (*var));
    return ret;
}

static inline void Atomic_Write32(unsigned *var, unsigned val)
{
    asm ("mov %0, %1"
         : "=m" (*var)
         : "ri" (val));
}
