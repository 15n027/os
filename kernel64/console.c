#include <stddef.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include "basic_defs.h"
#include "kassert.h"
#include "debug.h"
#include "x86/x86_defs.h"
#include "serial.h"
#include "smp.h"

static uint16_t *vga_txt_mem = (uint16_t*)0xb8000;
static const int VGA_TXT_BUF_SIZE = 80 * 25 * 2;
static int col, line;

static size_t earlyconsole_write(int fd, const void *buf, size_t len);
size_t (*console_write)(int fd, const void *buf, size_t len) = earlyconsole_write;

void
Log(const char *fmt, ...)
{
    va_list val;
    bool ints;
    static spinlock logLock;

    va_start(val, fmt);
    ints = spin_lock(&logLock);
    vprintf(fmt, val);
    spin_unlock(&logLock, ints);
    va_end(val);
}

static inline void
mmio_write8(volatile void *mmio, unsigned char val)
{
    *(unsigned char volatile *)mmio = val;
}

static inline void
mmio_write16(volatile void *mmio, uint16_t val)
{
    *(uint16_t volatile *)mmio = val;
}

void
debug_outs(const void *buf, size_t len)
{
    const char *p = buf;
    for (size_t i = 0; i < len; i++) {
        OUTB(0x39, p[i]);
        if (p[i] == '\n') {
            serial_putchar(1, '\r');
        }
        serial_putchar(1, p[i]);
    }
}

void
debug_out(const char *fmt, ...)
{
    char buf[160];
    va_list val;
    int ret;
    va_start(val, fmt);
    ret = vsnprintf(buf, sizeof buf, fmt, val);
    va_end(val);
    ASSERT(ret != -1);
    debug_outs(buf, MIN(ret, sizeof buf - 1));
}

size_t
__kernel_write(int fd, const void *buf, size_t len)
{
    size_t ret;

    debug_outs(buf, len);
    ret = console_write(fd, buf, len);
    return ret;
}

static void
earlyconsole_scroll(int lines)
{
    memmove(vga_txt_mem, vga_txt_mem + 80 * lines, VGA_TXT_BUF_SIZE - 80 * 2 * lines);
    memset(vga_txt_mem + 80 * (25 - lines), 0, VGA_TXT_BUF_SIZE - (25 - lines) * 80 * 2);
}

static size_t
earlyconsole_write(int fd, const void *buf, size_t len)
{
static const int VGA_MAX_LINE = 24;
    const char *s = buf;
    size_t i;
    for (i = 0; i < len; i++) {
        if (line > VGA_MAX_LINE) {
            earlyconsole_scroll(1);
            line = VGA_MAX_LINE;
        }
        switch (s[i]) {
        case '\n':
            line++;
            col = 0;
            break;
        case '\r':
            col = 0;
            break;
        case '\t':
            col += 4;
            break;
        default:
            if (col > 79) {
                col = 0;
                line++;
                if (line > VGA_MAX_LINE) {
                    earlyconsole_scroll(1);
                    line = VGA_MAX_LINE;
                }
            }
            mmio_write16(vga_txt_mem + col + line * 80, (0x7<<8) | s[i]);
            col++;
            break;
        }
    }
    return len;
}

void
earlyconsole_init(void)
{
    int i, j;
    int c;
    serial_init(1);
    for (i = 0; i < 25; i++) {
        for (j = 0; j < 80; j++) {
            c = vga_txt_mem[i * 80 + j] & 0xff;
            if (!isspace(c)) {
                line = i + 1;
                break;
            } else {
                vga_txt_mem[i * 80 + j] |= 0xf00;
            }
        }
    }
}

