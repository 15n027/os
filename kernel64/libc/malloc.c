#include <stdio.h>
#include <stddef.h>
#include "memmap.h"

void *
__kernel_alloc_va_pages(size_t n)
{
    VA va = alloc_va(NULL, n);
    //    DBG("va=%lx n=%lu", va, n);
    return (void*)va;
}
