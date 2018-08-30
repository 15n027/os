#pragma once
#include <limits.h>

#define KB (1024ull)
#define MB (1024ull * KB)
#define GB (1024ull * MB)

#define BYTES_TO_KB(x) ((x) / KB)
#define BYTES_TO_MB(x) ((x) / MB)
#define BYTES_TO_GB(x) ((x) / GB)
#define BYTES_TO_PAGES(x) ((x) / PAGE_SIZE)
#define PAGES_SPANNED(x, y) ((ROUNDUP(y, PAGE_SIZE) - ROUNDDOWN(x, PAGE_SIZE)) / PAGE_SIZE)
#define LPAGES_SPANNED(x, y) ((ROUNDUP(y, LPAGE_SIZE) - ROUNDDOWN(x, LPAGE_SIZE)) / LPAGE_SIZE)

#define container_of(ptr, type, member) ({                          \
            const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
            (type *)( (char *)__mptr - offsetof(type,member) );})

#define STRINGIZE(x) #x
#define BIT(x) (1ull << (x))
#define MIN(x, y) ({typeof(x) xx = x; typeof(y) yy = y; xx < yy ? xx : yy; })
#define MAX(x, y) ({typeof(x) xx = x; typeof(y) yy = y; xx > yy ? xx : yy; })

#define HIWORD(x) ((x) >> 16)
#define LOWORD(x) ((x) & 0xffff)
#define HIDWORD(x) ((x) >> 32)
#define LODWORD(x) ((x) & 0xffffffff)
#define QWORD(x, y) ((((uint64_t)(x)) << 32) | (y))

#define ARRAYSIZE(x) (sizeof (x) / sizeof ((x)[0]))
#define ISP2(x) ({typeof(x) xx = (x); ((xx) & (xx - 1)) == 0;})

#define ROUNDUP(x, y) ({typeof(x) xx = (x); typeof(y) yy = (y); ((xx + yy - 1) / yy) * yy;})
#define ROUNDDOWN(x, y) ({typeof(x) xx = (x); typeof(y) yy = (y); (xx / yy) * yy;})

#define ALIGN(x, y) ROUNDDOWN(x, y)
#define IS_ALIGNED(x, y) (((x) & ((y) - 1)) == 0)

#define ALIGNED(x) __attribute__((aligned(x)))
#define PACKED __attribute__((packed))

#define LIKELY(x)       __builtin_expect((x),1)
#define UNLIKELY(x)     __builtin_expect((x),0)

static inline unsigned
lssb64(unsigned long long u)
{
    unsigned ret;
    asm("bsf %1, %q0\n"
        "jnz 1f\n"
        "movl $64, %0\n"
        "1:\n"
        : "=r" (ret)
        : "rm" (u)
        : "cc");
    return ret;
}

static inline unsigned
lscb64(unsigned long long u)
{
    return lssb64(~u);
}

static inline unsigned
lssb32(unsigned u)
{
    unsigned ret;
    asm("bsf %1, %0\n"
        "jnz 1f\n"
        "movl $64, %0\n"
        "1:\n"
        : "=r" (ret)
        : "rm" (u)
        : "cc");
    return ret;
}

static inline unsigned
lscb32(unsigned u)
{
    return lssb32(~u);
}
