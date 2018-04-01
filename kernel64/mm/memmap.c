#include "memmap.h"
#include "x86/paging.h"

#include <inttypes.h>
#include <stdio.h>
#include "kernel.h"
#include "basic_types.h"
typedef struct {
    uint64 start;
    uint64 end;
    uint64 virtOffset;
    void **freeList;
} PagePool;

#define MAX_POOLS PAGE_SIZE / sizeof(PagePool)
static PagePool pools[MAX_POOLS];
static size_t poolCnt;

static void
insert_range(uint64 start, uint64 end)
{
    pools[poolCnt].start = start;
    pools[poolCnt].end = end;
    pools[poolCnt].freeList = NULL;
    pools[poolCnt].virtOffset = 0;
    printf("insert_Range: %016lx - %016lx\n", start, end);
    poolCnt++;
}

static void
apply_used_range(uint64 start, uint64 end)
{
    size_t i;
    start = ROUNDDOWN(start, PAGE_SIZE);
    end = ROUNDUP(end, PAGE_SIZE);
    for (i = 0; i < poolCnt; i++) {
        pools[i].start = ROUNDUP(pools[i].start, PAGE_SIZE);
        pools[i].end = ROUNDDOWN(pools[i].end, PAGE_SIZE);
        if (start <= pools[i].start && end >= pools[i].start) {
            printf("overlap left: %016lx - %016lx, %016lx - %016lx\n",
                    start, end, pools[i].start, pools[i].end);
            pools[i].start = end;
        } else if (start < pools[i].end && end >= pools[i].end) {
            printf("overlap right: %016lx - %016lx, %016lx - %016lx\n",
                    start, end, pools[i].start, pools[i].end);
            pools[i].end = start;
        } else if (start > pools[i].start && end < pools[i].end) {
            printf("punch hole: %016lx - %016lx, %016lx - %016lx\n",
                    start, end, pools[i].start, pools[i].end);
            insert_range(end, pools[i].end);
            pools[i].end = start;
        }
    }
}

static void
sanitize_pmm_ranges(void)
{
    size_t i, j;
    VA va;
    apply_used_range(0xb8000, 0xb9000);
    for (va = START_KERNEL; va < END_KERNEL; va += PAGE_SIZE) {
        PA pa = vtophys(VA_TO_PTR(va));
        apply_used_range(pa, pa + PAGE_SIZE);
    }
    for (i = 0; i < poolCnt; i++) {
        for (j = i + 1; j < poolCnt; j++) {
            if (pools[j].start < pools[i].start) {
                PagePool tmp = pools[i];
                pools[i] = pools[j];
                pools[j] = tmp;
            }
        }
    }
    puts("PMM Ranges:");
    for (i = 0; i < poolCnt; i++) {
        printf("[ %016"PRIx64" - %016"PRIx64"]\n", pools[i].start, pools[i].end);
    }
}

void pmm_init_multiboot(multiboot_info_t *mbi)
{
    const multiboot_memory_map_t *mmap = NULL;

    while ((mmap = next_multiboot_mmap_entry(mbi, mmap)) != NULL) {
        if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE) {
            insert_range(mmap->addr, mmap->addr + mmap->len);
        }
    }
    apply_used_range(PTR_TO_VA(mbi), PTR_TO_VA(mbi) + sizeof *mbi);
    sanitize_pmm_ranges();
}


PA
alloc_aligned_phys_pages_in_range(PA loAddr, PA hiAddr, size_t nPages, uintptr_t align)
{
    uint32 i;
    uint64 len = nPages * PAGE_SIZE;

    align = align == 0 ? PAGE_SIZE : align;
    ASSERT(IS_ALIGNED(align, PAGE_SIZE));
    loAddr = ROUNDUP(loAddr, align);
    for (i = 0; i < poolCnt; i++) {
        PagePool *p = &pools[i];
        uint64 start = MAX(ROUNDUP(p->start, align), loAddr);
        uint64 end = start + len;
        if (end > hiAddr || end > p->end) {
            continue;
        }
        p->start = end;
        return start;
    }
    return INVALID_PA;
}

PA
alloc_aligned_phys_pages(size_t nPages, uintptr_t align)
{
    return alloc_aligned_phys_pages_in_range(0, ~0ull, nPages, align);
}

PA
alloc_phys_pages(size_t nPages)
{
    PA ret;

    ret = alloc_aligned_phys_pages_in_range(4 * GB, ~0ull, nPages, 0);
    if (ret == INVALID_PA) {
        return alloc_aligned_phys_pages_in_range(0, ~0ull, nPages, 0);
    }
    return ret;
}

PA
alloc_phys_page(void)
{
    PA ret;

    ret = alloc_aligned_phys_pages_in_range(4 * GB, ~0ull, 1, 0);
    if (ret == INVALID_PA) {
        return alloc_aligned_phys_pages_in_range(0, ~0ull, 1, 0);
    }
    return ret;
}
