#include <stddef.h>
#include "kernel.h"

static void *alloc_earlyheap(size_t pages);
static void *(*alloc_fn)(size_t pages) = alloc_earlyheap;

static void *alloc_earlyheap(size_t pages)
{
    static VA brk = S_EARLYHEAP;
    VA ret = brk;

    if (brk + pages * PAGE_SIZE <= E_EARLYHEAP) {
        brk += pages * PAGE_SIZE;
        return VA_TO_PTR(ret);
    } else {
        return NULL;
    }
}


void *__kernel_alloc_va_pages(size_t pages)
{
    return alloc_fn(pages);
}
