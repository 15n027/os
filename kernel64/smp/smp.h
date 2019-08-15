#pragma once
#include "x86/x86_defs.h"
#include "katomic.h"
#include "x86/apic.h"

extern uint32 *cpuToApicMap;

uint32 cpuToApic(uint32 cpu);
void BootAP(uint32 apicId);

typedef struct spinlock {
    volatile uint32 lock;
    const char *fn;
} spinlock;

static inline bool spin_lock_impl(spinlock *lck, const char *fn)
{
    uint32 lockVal = MyApicId() + 1;
    uint64 flags;
    asm("pushf;"
        "pop %0"
        : "=r" (flags));
    if ((flags & EFLAGS_IF) != 0) {
        asm("cli");
    }
    while (Atomic_CMPXCHG((uint32*)&lck->lock, 0, lockVal) != 0) {
        PAUSE();
    }
    lck->fn = fn;
    return flags & EFLAGS_IF;
}
#define spin_lock(lck) spin_lock_impl((lck), __func__)

static inline void spin_unlock(spinlock *lck, bool ints)
{
    ASSERT(Atomic_Read32((uint32*)&lck->lock) == MyApicId() + 1);
    Atomic_Write32((uint32*)&lck->lock, 0);
    if (ints) {
        asm("sti");
    }
}
