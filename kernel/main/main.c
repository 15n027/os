#include <string.h>
#include <stdio.h>
extern void *ctors_begin;
extern void *ctors_end;

uint16_t *vga=(void*)0xb8000;
int main(void)
{
    void **p = &ctors_begin;
    printf("bort\n");
    memset(vga, 0, 25*80*2);
    while (p != &ctors_end) {
        void (*fn)(void) = (void(*)(void))*p;
        fn();
        p++;
    }
    asm("cli\nhlt\n");
    for(;;);
    return 0;
}
