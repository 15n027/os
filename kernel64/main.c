#include <stdio.h>

void
earlyconsole_init(void);

void
kern_entry(void)
{
    earlyconsole_init();
    printf("made it to 64 bit mode woot\n");
    fflush(0);
    asm("cli;hlt");
}
