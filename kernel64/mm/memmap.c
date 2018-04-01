#include "memmap.h"
#include "x86/paging.h"
#include "x86/x86_defs.h"
#include <inttypes.h>
#include <stdio.h>
#include "kernel.h"
#include "basic_types.h"
#include "scratch.h"

typedef struct {
    uint64 start;
    uint64 end;
} PagePool;

#define MAX_POOLS PAGE_SIZE / sizeof(PagePool)
static PagePool pools[MAX_POOLS];
static size_t poolCnt;

#define IDX_TO_VA(l4, l3, l2, l1)                             \
    (((l4 >= 256) ? 0xffff000000000000ull : 0) |              \
      ((uint64)l4 << PML_SHIFT(4)) |                          \
      ((uint64)l3 << PML_SHIFT(3)) |                          \
      ((uint64)l2 << PML_SHIFT(2)) |                          \
      ((uint64)l1 << PML_SHIFT(1)))

void
invtlb(void)
{
    SET_CR3(GET_CR3());
}

void
invpage(void *va)
{
    asm volatile("invlpg (%0)" : : "r"(va) : "memory");
}

#define PTIDX 256
const PTE *pml[] = {NULL,
                    (PTE*)IDX_TO_VA(PTIDX, 0, 0, 0),
                    (PTE*)IDX_TO_VA(PTIDX, PTIDX, 0, 0),
                    (PTE*)IDX_TO_VA(PTIDX, PTIDX, PTIDX, 0),
                    (PTE*)IDX_TO_VA(PTIDX, PTIDX, PTIDX, PTIDX)};
static PA
pagewalk(void *p)
{
    PA ret;
    uint32 i;
    PTE pte;
    uint64 off = 0;

    for (i = 4; i > 0; i--) {
        off = off * 512 + PML_OFF((VA)p, i);
        pte = pml[i][off];
        if ((pte & PT_P) == 0) {
            DBG("%p NP", p);
            ASSERT(0);
            return -1;
        }
        if (pte & PT_PS) {
            ret = pte & ~(PT_NX | LPAGE_MASK);
            return ret;
        }
    }
    ret = pte & ~(PT_NX | PAGE_MASK);
    return ret;
}

static void
insert_range(uint64 start, uint64 end)
{
    ASSERT(poolCnt < MAX_POOLS);
    pools[poolCnt].start = start;
    pools[poolCnt].end = end;
    printf("insert_Range: %016lx - %016lx\n", start, end);
    poolCnt++;
}

static void
apply_used_range(uint64 start, uint64 end)
{
    size_t i;
    const bool verbose = 0;
    start = ROUNDDOWN(start, PAGE_SIZE);
    end = ROUNDUP(end, PAGE_SIZE);
    for (i = 0; i < poolCnt; i++) {
        pools[i].start = ROUNDUP(pools[i].start, PAGE_SIZE);
        pools[i].end = ROUNDDOWN(pools[i].end, PAGE_SIZE);
        if (start <= pools[i].start && end >= pools[i].start) {
            if (verbose) {
                printf("overlap left: %016lx - %016lx, %016lx - %016lx\n",
                        start, end, pools[i].start, pools[i].end);
            }
            pools[i].start = end;
        } else if (start < pools[i].end && end >= pools[i].end) {
            if (verbose) {
                printf("overlap right: %016lx - %016lx, %016lx - %016lx\n",
                        start, end, pools[i].start, pools[i].end);
            }
            pools[i].end = start;
        } else if (start > pools[i].start && end < pools[i].end) {
            if (verbose) {
                printf("punch hole: %016lx - %016lx, %016lx - %016lx\n",
                        start, end, pools[i].start, pools[i].end);
            }
            insert_range(end, pools[i].end);
            pools[i].end = start;
        }
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

static void
sanitize_pmm_ranges(void)
{
    size_t i, j;
    VA va;
    apply_used_range(0xb8000, 0xb9000);
    for (va = ROUNDDOWN(START_KERNEL, PAGE_SIZE); va < ROUNDUP(END_KERNEL, PAGE_SIZE); va += PAGE_SIZE) {
        PA pa = pagewalk(VA_TO_PTR(va));
        ASSERT(pa != -1);
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

void
pmm_init_multiboot(multiboot_info_t *mbi)
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
