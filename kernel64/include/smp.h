#pragma once
#include "x86/x86_defs.h"
#include "katomic.h"
#include "apic.h"

extern uint32 *cpuToApicMap;

uint32 cpuToApic(uint32 cpu);
void BootAP(uint32 apicId);

typedef struct spinlock {
    uint32 lock;
} spinlock;

static inline bool spin_lock(spinlock *lck)
{
    uint32 val = MyApicId();
    bool intsOn = INTERRUPTS_ENABLED();

    do {
        if (intsOn) {
            ENABLE_INTERRUPTS();
        }
        ASSERT(Atomic_Read32(&lck->lock) != val);
        while (Atomic_Read32(&lck->lock) != 0) {
            PAUSE();
        }
        DISABLE_INTERRUPTS();
        asm("xchg %0, %1"
            : "+r"(val), "+m"(lck->lock)
            :
            : "memory");
    }
    while (val != 0);
    return intsOn;
}

static inline void spin_unlock(spinlock *lck, bool intsOn)
{
    ASSERT(Atomic_Read32(&lck->lock) == MyApicId());
    Atomic_Write32(&lck->lock, 0);
    if (intsOn) {
        ENABLE_INTERRUPTS();
    }
}
