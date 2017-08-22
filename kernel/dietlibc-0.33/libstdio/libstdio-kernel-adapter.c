#include <stdio.h>
int fflush(FILE *stream)
{
    return 0;
}
int lseek(int fd, off_t off, int whence)
{
    return 0;
}

int isatty(int fd)
{
    return 1;
}
