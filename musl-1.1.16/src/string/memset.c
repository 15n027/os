#include <string.h>
#include <stdint.h>

void *memset(void *dest, int c, size_t n)
{
    void *tmp = dest;
    asm volatile("rep; stosb\n"
            : "+D"(dest), "+a"(c), "+c"(n) : : "memory", "cc");
    return tmp;
}
