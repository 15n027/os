#pragma once
#include "basic_types.h"

enum {
    UART_THR = 0,  /* Transmitter holding buffer */
    UART_RBR = 0,  /* Receive buffer */
    UART_DLL = 0,  /* Divisor latch low */
    UART_IER = 1,  /* Interrupt enable register */
    UART_DLH = 1,  /* Divisor latch high */
    UART_IIR = 2,  /* Interrupt identification register */
    UART_FCR = 2,  /* FIFO control register */
    UART_LCR = 3,  /* Line control register */
    UART_MCR = 4,  /* Modem control register */
    UART_LSR = 5,  /* Line status register */
    UART_MSR = 6,  /* Modem status register */
    UART_SR  = 7,  /* Scratch register */
};

enum {
    UART_IER_ERBFI = (1 << 0), /* Enable received data avail interrupt */
    UART_IER_ETBEI = (1 << 1), /* Enable xmit holding register empty interrupt */
    UART_IER_ELSI  = (1 << 2), /* Enable recv line status interrupt */
    UART_IER_EDSSI = (1 << 3), /* Enable modem status interrupt */

    UART_IIR_PND = (1 << 0), /* Interrupt pending, active-low */

};

#define COM1_BASE 0x3f8
#define COM2_BASE 0x2f8
#define COM3_BASE 0x3e8
#define COM4_BASE 0x2e8

static inline uint32 COM_BASE(uint32 com)
{
    switch (com) {
    case 1:  return COM1_BASE;
    case 2:  return COM2_BASE;
    case 3:  return COM3_BASE;
    case 4:  return COM4_BASE;
    default: NOT_REACHED();
    }
}

void serial_putchar(uint32 com, uint8 ch);
void serial_puts(uint32 com, const char *s);
bool serial_init(uint32 com);
bool serial_lateinit(void);
