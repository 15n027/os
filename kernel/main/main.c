#include <string.h>
#include <stdio.h>
extern void *ctors_begin;
extern void *ctors_end;

int main(void)
{
    void **p = &ctors_begin;
    printf("bort\n");
    while (p != &ctors_end) {
        void (*fn)(void) = (void(*)(void))*p;
        fn();
        p++;
    }
    memset((void*)0x12345, 0x12, 99);
    asm("cli\nhlt\n");
    for(;;);
    return 0;
}
