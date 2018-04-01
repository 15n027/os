#include "kassert.h"
#include "basic_defs.h"
#include "basic_types.h"

void __lock(volatile int *lck)
{
    ASSERT(IS_ALIGNED(PTR_TO_VA(lck), sizeof(*lck)));
    ASSERT(*lck == 0);
    do {
        asm volatile("lock; cmpxchgl %1, %0\n"
                : "=m"(*lck)
                : "r"(1), "a"(0)
                : "cc");
    } while (*lck == 0);
}

void __unlock(volatile int *lck)
{
    ASSERT(IS_ALIGNED(PTR_TO_VA(lck), sizeof(*lck)));
    ASSERT(*lck != 0);

    asm volatile("movl $0, %0\n" : : "m"(*lck));
}
