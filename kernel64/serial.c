#include "serial.h"
#include "x86/x86_defs.h"
#include "interrupt.h"
#include "apic.h"

#define SET_DLAB(com) OUTB(COM_BASE(com) + UART_LCR, INB(COM_BASE(com) + UART_LCR) | 0x80)
#define CLR_DLAB(com) OUTB(COM_BASE(com) + UART_LCR, INB(COM_BASE(com) + UART_LCR) & ~0x80)

void __attribute__((noinline)) serial_putchar(uint32 com, uint8 ch)
{
    while ((INB(COM_BASE(com) + UART_LSR) & 0x20) != 0x20) {
        PAUSE();
    }
    OUTB(COM_BASE(com) + UART_THR, ch);
}

void serial_puts(uint32 com, const char *s)
{
    while (*s) {
        serial_putchar(com, *s);
        s++;
    }
}

static bool
serial_input(IntrFrame64 *frame)
{
    extern size_t (*console_write)(int fd, const void *buf, size_t len);

    while ((INB(COM1_BASE + UART_LSR) & 1) != 0) {
        uint8 ch = INB(COM1_BASE + UART_RBR);
        serial_putchar(1, ch);
        if (ch == '\r') {
            char newline = '\n';
            serial_putchar(1, '\n');
            console_write(1, &newline, 1);
        } else {
            console_write(1, &ch, 1);
        }
    }
    //    printf("IIR=%x LSR=%x\n", INB(COM1_BASE + UART_IIR), INB(COM1_BASE + UART_LSR));
    apic_write(APIC_EOI, 0);
    return true;
}

bool serial_init(uint32 com)
{
#define REG(x) (COM_BASE(com) + (x))

    OUTB(COM1_BASE + UART_IER, 0x0);
    OUTB(COM2_BASE + UART_IER, 0x0);
    OUTB(COM3_BASE + UART_IER, 0x0);
    OUTB(COM4_BASE + UART_IER, 0x0);

    SET_DLAB(com);
    OUTB(REG(UART_DLH), 0);
    OUTB(REG(UART_DLL), 1);
    CLR_DLAB(com);
    OUTB(REG(UART_LCR), 0x3); // 8-n-1
    OUTB(REG(UART_FCR), 0xc7);
    OUTB(REG(UART_MCR), 0xb);
    return true;
}

bool serial_lateinit(void)
{
    install_handler(36, serial_input);
    OUTB(COM1_BASE + UART_IER, UART_IER_ERBFI);
    return true;
}
