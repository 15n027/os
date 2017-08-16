#pragma once

#include "basic_defs.h"

#define PORT_COM1 0x3f8
#define PORT_COM2 0x2f8

#define UART_REG_RBR 0
#define UART_REG_THR 0
#define UART_REG_IER 1
#define UART_REG_DL_LO 0
#define UART_REG_DL_HI 1
#define UART_REG_IIR 2
#define UART_REG_FCR 2
#define UART_REG_LCR 3
#define UART_REG_MCR 4
#define UART_REG_LSR 5
#define UART_REG_MSR 6
#define UART_REG_SCR 7

#define UART_FLAG_EDSSI BIT(3)
#define UART_FLAG_ELSI BIT(2)
#define UART_FLAG_ETBEI BIT(1)
#define UART_FLAG_ERBFI BIT(0)
#define UART_FLAG_FIFOE 

void uart_init(void);
