#include "serial.h"
#include "x86/x86_defs.h"


void serial_putchar(uint32 com, uint8 ch)
{
    while ((INB(COM_BASE(com) + UART_LSR) & 0x20) != 0x20);
    OUTB(COM_BASE(com) + UART_THR, ch);
}

void serial_puts(uint32 com, const char *s)
{
    while (*s) {
        serial_putchar(com, *s);
        s++;
    }
}

#define SET_DLAB(com) OUTB(COM_BASE(com) + UART_LCR, INB(COM_BASE(com) + UART_LCR) | 0x80)
#define CLR_DLAB(com) OUTB(COM_BASE(com) + UART_LCR, INB(COM_BASE(com) + UART_LCR) & ~0x80)
bool serial_init(uint32 com)
{
#define REG(x) (COM_BASE(com) + (x))
    SET_DLAB(com);
    OUTB(REG(UART_DLH), 0);
    OUTB(REG(UART_DLL), 1);
    CLR_DLAB(com);
    OUTB(REG(UART_LCR), 0x3); // 8-n-1
    OUTB(REG(UART_IER), 0);
    OUTB(REG(UART_FCR), 0xc7);
    return true;
}
