#pragma once
#include <stdbool.h>
#include <stdio.h>
#include "debug.h"

#define ASSERT_ON_COMPILE1(x) x
#define ASSERT_ON_COMPILE(cond) ((void)sizeof(char[(cond) ? 1 : -1]))

#define VERIFY(x)                                   \
    do {                                            \
        if (!(x)) {                                 \
            kverify_fail(__FILE__, __LINE__, #x);   \
        }                                           \
    } while (0)

#if defined(INCLUDE_ASSERTS)
#define ASSERT(x)                                   \
    do {                                            \
        if (!(x)) {                                 \
            kassert_fail(__FILE__, __LINE__, #x);   \
        }                                           \
    } while (0)
#else
#define ASSERT(x) ((void)0)
#endif

#define IMPLIES(x, y) (!(x) || (y))
#define NOT_REACHED()                             \
    do {                                          \
        VERIFY(!"unreachable");                   \
    } while (0)
#define NOT_IMPLEMENTED()                       \
    do {                                        \
        VERIFY(!"implemented");                 \
    } while (0)

#define Panic(msg, ...)                                                 \
    do {                                                                \
        printf("PANIC: %s:%d:" msg "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
        HALT();                                                         \
    } while (0)

#define PanicIf(cond, msg, ...)                 \
    do {                                        \
        if ((cond))                             \
            Panic(msg, ##__VA_ARGS__);           \
    } while (0)

static inline void kassert_fail(const char *filename, int line, const char *cond)
{
    static bool recurse;
    if (recurse != 0) {
        goto halt;
    }
    recurse = 1;
    printf("ASSERT: %s:%d (%s)\n", filename, line, cond);
halt:
    for (;;) {
        HALT();
    }
}
static inline void kverify_fail(const char *filename, int line, const char *cond)
{
    printf("VERIFY: %s:%d (%s)\n", filename, line, cond);
    for (;;)
        HALT();
    __builtin_unreachable();
}
