#include <stdio.h>
ssize_t write(int fd, const void *buf, size_t count)
{
    const char *c = buf;
    size_t i;
    for (i = 0; i < count; i++) {
        asm("out %0, $0xe9\n"
            :: "a"(c[i]));
    }
    return i;
}
