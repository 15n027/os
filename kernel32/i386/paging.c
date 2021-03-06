#include <string.h>
#include "basic_types.h"
#include "i386/paging32.h"
#include "x86/x86_defs.h"
#include "kernel.h"
#include "pagealloc.h"

static bool pagingEnabled;
static uint64 pml4[PAGE_SIZE / sizeof(uint64)] ALIGNED(PAGE_SIZE);
static uint64 pml3[PAGE_SIZE / sizeof(uint64)] ALIGNED(PAGE_SIZE);
static uint64 pml2[PAGE_SIZE / sizeof(uint64)] ALIGNED(PAGE_SIZE);
static uint64 pml1[PAGE_SIZE / sizeof(uint64)] ALIGNED(PAGE_SIZE);
static uint64 scratch[PAGE_SIZE / sizeof(uint64)] ALIGNED(PAGE_SIZE);


void
enable_paging(VA root)
{
    SET_CR3(root);
    SET_CR4(GET_CR4() | CR4_PAE);
    SET_CR0(GET_CR0() | CR0_PG | CR0_WP);
    pagingEnabled = TRUE;
}

bool
is_paging_enabled(void)
{
    return pagingEnabled;
}

PA
get_paging_root(void)
{
    return PTR_TO_PA(pml4);
}

void
init_4level_pagetable(void)
{
    ASSERT(ETEXT <= S_RW_AREA);
    ASSERT(S_RW_AREA <= SDATA);
    ASSERT(S_RW_AREA <= SBSS);
    ASSERT_ON_COMPILE(sizeof scratch == PAGE_SIZE);
    ASSERT(IS_ALIGNED(PTR_TO_PA(scratch), PAGE_SIZE));

    pml4[PML4_OFF(scratch)] = PT_ADDR_4K(PTR_TO_PA(pml3)) | PT_RW | PT_P;
    pml4[PT_RECURSIVE_IDX] = PT_ADDR_4K(PTR_TO_PA(pml4)) | PT_RW | PT_P;
    pml3[PML3_OFF(scratch)] = PT_ADDR_4K(PTR_TO_PA(pml2)) | PT_RW | PT_P;
    pml2[PML2_OFF(scratch)] = PT_ADDR_4K(PTR_TO_PA(pml1)) | PT_RW | PT_P;
    pml1[PML1_OFF(scratch)] = PT_ADDR_4K(PTR_TO_PA(scratch)) | PT_RW | PT_P;
    ASSERT(PML4_OFF(scratch) == 0);
    ASSERT(PML3_OFF(scratch) == 0);
    ASSERT(PML2_OFF(scratch) == 0);

    map_pages_with_root(get_paging_root(), START_KERNEL, START_KERNEL,
            PAGES_SPANNED(START_KERNEL, S_RW_AREA), PT_P);
    map_pages_with_root(get_paging_root(), S_RW_AREA, S_RW_AREA,
            PAGES_SPANNED(S_RW_AREA, END_KERNEL), PT_NX | PT_RW | PT_P);
    map_page_with_root(get_paging_root(), 0xb8000, 0xb8000, PT_NX | PT_RW | PT_P);
}

static void *
map_scratch(PA pa)
{
    if (is_paging_enabled()) {
        pml1[PML1_OFF(scratch)] = PT_ADDR_4K(pa) | PT_NX | PT_RW | PT_P;
        invpage(scratch);
        return scratch;
    } else {
        return PA_TO_PTR(pa);
    }
}

PA
pt_entry_to_pa(PTE entry)
{
    uint64 mask = PT_NX;
    ASSERT(entry & PT_P);
    if (entry & PT_PS) {
        mask |= LPAGE_MASK;
    } else {
        mask |= PAGE_MASK;
    }
    return entry & ~mask;
}

void
map_page_with_root(VA vRoot, VA64 vBase, PA pBase, uint64 flags)
{
    PA map = vRoot;
    uint32 lvl;
    uint64 *scratch;

    ASSERT(pBase != 0);
    ASSERT(IS_ALIGNED(vRoot, PAGE_SIZE));
    vBase = ALIGN(vBase, PAGE_SIZE);
    pBase = ALIGN(pBase, PAGE_SIZE);

    for (lvl = 4; lvl > 1; lvl--) {
        PTE *cur;
        scratch = map_scratch(map);
        cur = &scratch[PML_OFF(vBase, lvl)];
        if ((*cur & PT_P) == 0) {
            PA newpage = alloc_phys_page();
            printf("alloc PT%u page %llx\n", lvl, (uint64)(newpage));
            ASSERT(newpage != INVALID_PA);
            scratch = map_scratch(newpage);
            memset(scratch, 0, PAGE_SIZE);
            scratch = map_scratch(map);
            *cur = PT_ADDR_4K(newpage) | PT_P | PT_RW;
            map = newpage;
            invtlb();
        } else {
            ASSERT((*cur & PT_PS) == 0);
            map = PT_ENTRY_TO_PA(*cur);
        }
    }
    scratch = map_scratch(map);
    scratch[PML1_OFF(vBase)] = PT_ADDR_4K(pBase) | flags;

    invpage(VA_TO_PTR(vBase));
}

void
map_pages_with_root(VA vRoot, VA64 vBase, PA pBase, uint32 numPages, uint64 flags)
{
    uint32 i;

    for (i = 0; i < numPages; i++) {
        map_page_with_root(vRoot, vBase + i * PAGE_SIZE, pBase + i * PAGE_SIZE, flags);
    }
}

void
map_pages(VA vBase, PA pBase, uint32 numPages, uint64 flags)
{
    map_pages_with_root(get_paging_root(), vBase, pBase, numPages, flags);
}

void
map_page(VA vBase, PA pBase, uint64 flags)
{
    map_pages_with_root(get_paging_root(), vBase, pBase, 1, flags);
}
