#include "kern_sched.h"
#include "kassert.h"
#include "x86/x86_defs.h"
#include "memmap.h"
#include "kernel.h"
#include <stdlib.h>
#include <string.h>

#define USER_CS (4 << 3)
#define USER_DS (5 << 3)

Thread *threadList;
Thread *current;

void sched(Thread *thr)
{
    if (current->next == current) {
        return;
    }
    asm volatile(
            "mov %%rax, 0x0(%0);"
            "mov %%rcx, 0x8(%0);"
            "mov %%rdx, 0x10(%0);"
            "mov %%rbx, 0x18(%0);"
            "mov %%rsp, 0x20(%0);"
            "mov %%rsi, 0x28(%0);"
            "mov %%rdi, 0x30(%0);"
            "mov %%r8,  0x38(%0);"
            "mov %%r9,  0x40(%0);"
            "mov %%r10, 0x48(%0);"
            "mov %%r11, 0x50(%0);"
            "mov %%r12, 0x58(%0);"
            "mov %%r13, 0x60(%0);"
            "mov %%r14, 0x68(%0);"
            "mov %%r15, 0x70(%0);"
            "movq $resume, 0x80(%0);"
            "resume:;"
            "cmp $0, %1;"
            "jnz 1f;"
            // to kernelmode
            "1:;"
            :
            : "r"(&thr->frame), "g"(thr->user)
            : "cc");
}

Thread *sched_next(void)
{
    return current->next;
}

static Thread *thread_init(uint8 *stack, size_t stackSize, const char *name)
{
    Thread *th = (Thread*)(stack + stackSize - 128);
    // update start.s if this fails
    ASSERT_ON_COMPILE(sizeof *th <= 128);
    memset(th, 0, sizeof *th);
    strncpy(th->name, name, sizeof th->name);
    th->stack = stack;
    th->cr3 = GET_CR3();
    th->frame = (void*)ROUNDDOWN((VA)th - sizeof *th->frame, 16);
    th->next = NULL;
    return th;
}

void sched_init(void)
{
    VA rsp;
    current = thread_init((uint8*)_stack0_bottom, STACK0_SIZE, "kern-0");
    asm volatile("mov %%rsp, %0" : "=g"(rsp));
    ASSERT(rsp < (VA)current);
    current->next = current;
}

Thread *thread_new(const char *name)
{
    uint8 *pages = (void*)alloc_va(get_kern_vma(), 2);
    Thread *thr = thread_init(pages, PAGE_SIZE * 2, name);
    memset(thr->frame, 0, sizeof *thr->frame);
    thr->frame->rsp = (VA)thr->frame;

    return thr;
}

void thread_enqueue(Thread *thr)
{
    Thread *next = current->next;
    current->next = thr;
    thr->next = next;
}
