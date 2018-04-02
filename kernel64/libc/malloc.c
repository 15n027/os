#include <stdio.h>
#include <stddef.h>
#include "memmap.h"

void *
__kernel_alloc_va_pages(size_t n)
{
    VA va;
    va = alloc_va_from(get_kern_vma(), VM_AREA_HEAP, n);
    DBG("va=%lx n=%lu", va, n);
    return (void*)va;
}
