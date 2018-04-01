#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "memmap.h"
#include "x86/x86_defs.h"
#include "x86/paging.h"
#include "kernel.h"

PA
vtophys(const void *v)
{

    return 0;
}

void
vmm_init(void)
{
}

void vmm_earlyinit(void)
{
    //PA cr3 = GET_CR3();
    //void *scratch = earlyheap_alloc(4);
    //ASSERT(scratch != NULL);
    //    clone_paging_root(cr3, scratch);
}
