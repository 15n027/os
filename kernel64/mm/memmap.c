#include "memmap.h"
#include "x86/paging.h"
#include "x86/x86_defs.h"
#include <inttypes.h>
#include <stdio.h>
#include "kernel.h"
#include "basic_types.h"

typedef struct {
    uint32 start;
    uint32 end;
} PagePool;

typedef struct PMMNode {
    struct PMMNode *left, *right;
    uint64 start, end;
} PMMNode;

#define MAX_POOLS PAGE_SIZE / sizeof(PagePool)
static PagePool pools[MAX_POOLS];
static uint32 poolCnt;


static void
insert_range_pagenum(uint32 start, uint32 end)
{
    ASSERT(start <= end);
    ASSERT(poolCnt < MAX_POOLS);
    pools[poolCnt].start = start;
    pools[poolCnt].end = end;

    poolCnt++;
}

static void
insert_range(uint64 start, uint64 end)
{
    printf("insert_Range: %08lx - %08lx\n", start, end);
    start = ROUNDUP(start, PAGE_SIZE);
    end = ROUNDDOWN(end, PAGE_SIZE);
    start /= PAGE_SIZE;
    end /= PAGE_SIZE;
    insert_range_pagenum(start, end);
}

static void
apply_used_range(uint64 start, uint64 end)
{
    static const bool verbose = 0;
    uint32 i;

    //    DBG("apply_used_range: %08lx - %08lx", start, end);
    start = ROUNDDOWN(start, PAGE_SIZE);
    end = ROUNDUP(end, PAGE_SIZE);
    start /= PAGE_SIZE;
    end /= PAGE_SIZE;

    ASSERT(start <= end && end <= UINT32_MAX);

    for (i = 0; i < poolCnt; i++) {
        if (start <= pools[i].start && end >= pools[i].start) {
            if (verbose) {
                printf("overlap left: %08lx - %08lx, %08x - %08x\n",
                        start, end, pools[i].start, pools[i].end);
            }
            pools[i].start = end;
        } else if (start < pools[i].end && end >= pools[i].end) {
            if (verbose) {
                printf("overlap right: %08lx - %08lx, %08x - %08x\n",
                        start, end, pools[i].start, pools[i].end);
            }
            pools[i].end = start;
        } else if (start > pools[i].start && end < pools[i].end) {
            if (verbose) {
                printf("punch hole: %08lx - %08lx, %08x - %08x\n",
                        start, end, pools[i].start, pools[i].end);
            }
            insert_range_pagenum(end, pools[i].end);
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
    VA kern_start = ROUNDDOWN(START_KERNEL, PAGE_SIZE);
    VA kern_end = ROUNDUP(END_KERNEL, PAGE_SIZE);
    VA va;

    for (va = kern_start; va < kern_end; va += PAGE_SIZE) {
        PA pa = vtophys(VA_TO_PTR(va));
        ASSERT(pa != -1);
        apply_used_range(pa, pa + PAGE_SIZE);
    }
    apply_used_range(0xb8000, 0xb9000);
    for (uint64 i = 0; i < 512; i++) {
        PA pa;
        if ((PML4[i] & (PT_P | PT_PS)) == PT_P) {
            pa = PT_ENTRY_TO_PA(PML4[i]);
            apply_used_range(pa, pa + PAGE_SIZE);
            for (uint64 j = 0; j < 512; j++) {
                PTE pte = PML3[i * 512 + j];
                if ((pte & (PT_PS | PT_P)) == PT_P) {
                    pa = PT_ENTRY_TO_PA(pte);
                    apply_used_range(pa, pa + PAGE_SIZE);
                    for (uint64 k = 0; k < 512; k++) {
                        PTE pte = PML2[i * 512 * 512 + j * 512 + k];
                        if ((pte & (PT_PS | PT_P)) == PT_P) {
                            pa = PT_ENTRY_TO_PA(pte);
                            apply_used_range(pa, pa + PAGE_SIZE);
                        }
                    }
                }
            }
        }
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
    uint32 rd, wr;
    bool onemb = false, fourgb = false;
    for (rd = 0, wr = 0; rd < poolCnt; rd++) {
        if (pools[rd].start < pools[rd].end) {
            if (!onemb && pools[rd].start >= 1 * MB / PAGE_SIZE) {
                printf("----- 1 MB -----\n");
                onemb = true;
            }
            if (!fourgb && pools[rd].start >= 4ull * GB / PAGE_SIZE) {
                printf("----- 4 GB ------\n");
                fourgb = true;
            }
            printf("[ %08x - %08x] %u pages\n", pools[rd].start, pools[rd].end, (pools[rd].end - pools[rd].start));
            pools[wr] = pools[rd];
            wr++;
        }
    }
    poolCnt = wr;
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
    vmm_earlyinit();
    //    apply_used_range(PTR_TO_VA(mbi), PTR_TO_VA(mbi) + sizeof *mbi);
    sanitize_pmm_ranges();
}


PA
alloc_aligned_phys_pages_in_range(PA loAddr, PA hiAddr, size_t nPages, uintptr_t align)
{
    uint32 i;

    align = align == 0 ? PAGE_SIZE : align;
    ASSERT(IS_ALIGNED(align, PAGE_SIZE));
    loAddr = ROUNDUP(loAddr, align);
    hiAddr /= PAGE_SIZE;
    loAddr /= PAGE_SIZE;
    align /= PAGE_SIZE;
    for (i = 0; i < poolCnt; i++) {
        PagePool *p = &pools[i];
        uint64 start = MAX(ROUNDUP(p->start, align), loAddr);
        uint64 end = start + nPages;
        if (end > hiAddr || end > p->end) {
            continue;
        }
        p->start = end;
        return start * PAGE_SIZE;
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
        ret = alloc_aligned_phys_pages_in_range(1 * MB, ~0ull, 1, 0);
        if (ret == INVALID_PA) {
            ret = alloc_aligned_phys_pages_in_range(0, ~0ull, 1, 0);
        }
    }
    return ret;
}
