#pragma once
#include "x86/x86_defs.h"
#include "katomic.h"
#include "x86/apic.h"

extern uint32 *cpuToApicMap;

uint32 cpuToApic(uint32 cpu);
void BootAP(uint32 apicId);

typedef struct spinlock {
    volatile uint32 lock;
} spinlock;

static inline void spin_lock(spinlock *lck)
{
    uint32 lockVal = MyApicId() + 1;
    while (Atomic_CMPXCHG((uint32*)&lck->lock, 0, lockVal) != 0) {
        PAUSE();
    }
}

static inline void spin_unlock(spinlock *lck)
{
    ASSERT(Atomic_Read32((uint32*)&lck->lock) == MyApicId() + 1);
    Atomic_Write32((uint32*)&lck->lock, 0);
}
