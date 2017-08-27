#pragma once

#include <stdio.h>
#include "debug.h"

#define ASSERT_ON_COMPILE1(x) x
#define ASSERT_ON_COMPILE(cond) ((void)sizeof(char[(cond) ? 1 : -1]))
#if defined(INCLUDE_ASSERTS)
#define ASSERT(x)                                   \
    do {                                            \
        if (!(x)) {                                 \
            kassert_fail(__FILE__, __LINE__, #x);   \
            __builtin_unreachable();                \
        }                                           \
    } while (0)
#else
#define ASSERT(x) ((void)0)
#endif

#define IMPLIES(x, y) (!(x) || (y))
#define NOT_REACHED()                             \
    do {                                          \
        ASSERT(!"unreachable");                   \
        __builtin_unreachable();                  \
    } while (0)

static inline void kassert_fail(const char *filename, int line, const char *cond)
{
    printf("ASSERT: %s:%d (%s)\n", filename, line, cond);
    HALT();
}
