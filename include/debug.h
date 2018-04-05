#pragma once
#include <stdio.h>

#define DBG(fmt, ...) do{printf("%s:%d:"fmt"\n", __FILE__, __LINE__, ##__VA_ARGS__); } while (0)
extern void debug_out(const char *fmt, ...) __attribute__((format (printf, 1, 2)));
static inline void HALT(void) { asm("cli" ::: "memory"); puts("CLI;HALT"); for(;;) asm("cli;hlt"); }
