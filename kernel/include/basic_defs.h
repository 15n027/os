#pragma once

#define CTOR __attribute__((constructor))

#define container_of(ptr, type, member) ({                          \
            const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
            (type *)( (char *)__mptr - offsetof(type,member) );})

#define BIT(x) (1ull << (x))
#define MIN(x, y) ({typeof(x) xx = x; typeof(y) yy = y; xx < yy ? xx : yy; })
#define MAX(x, y) ({typeof(x) xx = x; typeof(y) yy = y; xx > yy ? xx : yy; })
#define HIWORD(x) ((x) >> 16)
#define LOWORD(x) ((x) & 0xffff)
#define HIDWORD(x) ((x) >> 32)
#define LODWORD(x) ((x) & 0xffffffff)
#define QWORD(x, y) ((((uint64_t)(x)) << 32) | (y))
