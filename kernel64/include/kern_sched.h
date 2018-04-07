#pragma once

#include "x86/exception.h"

typedef struct Thread {
    struct Thread *next;
    IntrFrame64 *frame;
    uint8 *stack;
    PA cr3;
    bool user;
    char name[64];
} Thread;
extern Thread *processList;
extern Thread *current;

void sched(Thread *);
Thread *sched_next(void);
void sched_init(void);
Thread *thread_new(const char *name);
void thread_enqueue(Thread *proc);
