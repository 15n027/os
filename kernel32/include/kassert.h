#pragma once
#include <stdbool.h>
#include <stdio.h>
#include "basic_defs.h"

#define ASSERT(x)                                   \
    do {                                            \
        if (!(x)) {                                 \
            kassert_fail(__FILE__, __LINE__, #x);   \
            __builtin_unreachable();                \
        }                                           \
    } while (0)

#define IMPLIES(x, y) ((x) ? (y) : true)
#define NOT_REACHED() do { ASSERT("!unreachable"); __builtin_unreachable(); } while (0)

static inline void kassert_fail(const char *filename, int line, const char *cond)
{
    printf("ASSERT: %s:%d (%s)\n", filename, line, cond);
    asm("ud2");
}
