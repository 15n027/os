#include <string.h>

void *memcpy(void *restrict dest, const void *restrict src, size_t n)
{
    unsigned char *d = dest;
    asm volatile("rep;movsb\n"
            : "+D"(dest), "+S"(src), "+c"(n) : : "memory", "cc");
    return d;
}
