#pragma once
#include "basic_types.h"
#define START_KERNEL PTR_TO_PA(_start_kernel)
#define END_KERNEL   PTR_TO_PA(_end_kernel)
#define SBSS PTR_TO_PA(_sbss)
#define EBSS PTR_TO_PA(_ebss)
#define SDATA PTR_TO_PA(_sdata)
#define EDATA PTR_TO_PA(_edata)
#define STEXT PTR_TO_PA(_stext)
#define ETEXT PTR_TO_PA(_etext)
#define S_RW_AREA PTR_TO_PA(_s_rw_area)
#define S_EARLYHEAP PTR_TO_PA(_start_earlyheap)
#define E_EARLYHEAP (S_EARLYHEAP + PTR_TO_PA(_earlyheap_size))
#define STACK0_SIZE PTR_TO_PA(_stack0_size)
extern const char _start_kernel[], _end_kernel[];
extern const char _stack0_top[], _stack0_bottom[], _stack0_size[];
extern const uint32 _bss_size;
extern const char _sbss[], _ebss[];
extern const char _sdata[], _edata[];
extern const char _stext[], _etext[];
extern const char _s_rw_area[];
