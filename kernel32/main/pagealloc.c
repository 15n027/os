#include <stdlib.h>
#include "pagealloc.h"
#include "basic_defs.h"
#include "x86/x86_defs.h"
#include "kassert.h"
#include <limits.h>

#define MAX_POOL 64
static PagePool pools[MAX_POOL];
static uint32 allocBitmap[4 * GB / PAGE_SIZE / 32];

static int
pool_compare(const void *a, const void *b)
{
    const PagePool *p1 = a, *p2 = b;
    if (p1->start == p2->start) return 0;
    if (p1->len == 0) return 1;
    if (p2->len == 0) return -1;
    return p1->start < p2->start ? -1 : 1;
}
static void
pool_add(PA start, uint64 len)
{
    uint32 i;
    printf("%s: [ %08llx - %08llx ]\n", __func__, start, start + len);
    for (i = 0; i < ARRAYSIZE(pools); i++) {
        if (pools[i].len == 0) {
            pools[i].start = start;
            pools[i].len = len;
            return;
        }
    }
    NOT_REACHED();
}

void
complete_phys_map(void)
{
    uint32 i;
    PA start = 0;
    uint64 len = 0;
    for (i = 0; i < sizeof allocBitmap * CHAR_BIT; i++) {
        if (allocBitmap[i / 32] & (1u << (i % 32))) {
            if (len == 0) {
                start = i * PAGE_SIZE;
            }
            len += PAGE_SIZE;
        } else {
            if (len > 0) {
                pool_add(start, len);
            }
            len = 0;
        }
    }
    if (len > 0) {
        pool_add(start, len);
    }
    qsort(pools, ARRAYSIZE(pools), sizeof pools[0], pool_compare);
}

void
print_phys_map(void)
{
    uint32 i;
    for (i = 0; i < ARRAYSIZE(pools); i++) {
        if (pools[i].len > 0) {
            printf("Pool[%u] [ %016llx - %016llx ]\n", i, pools[i].start, pools[i].start + pools[i].len);
        }
    }
}

void
mark_phys_range(PA start, uint64 len, bool available)
{
    uint64 page = start >> PAGE_SHIFT;
    printf("%s: [ %08llx - %08llx ] AVAIL: %d\n", __func__, start, start + len, available);
    if (start + len > 4 * GB) {
        printf("over 4GB!\n");
        if (start < 4 * GB) {
            printf("%llx < %llx\n", start, 4 * GB);
            printf("4GB spanner!\n");
            HALT();
            mark_phys_range(start, 4 * GB - start, available);
            if (available) {
                pool_add(4 * GB, start + len - 4 * GB);
            }
        } else if (available) {
            pool_add(start, len);
        }
        return;
    }
    for (; page * PAGE_SIZE < start + len; page++) {
        uint32 *p = &allocBitmap[page / 32];
        if (available) {
            *p |= (1u << (page % 32));
        } else {
            *p &= ~(1u << (page % 32));
        }
    }
}

PA
alloc_aligned_phys_pages_in_range(PA loAddr, PA hiAddr, size_t nPages, uintptr_t align)
{
    uint32 i;
    uint64 len = nPages * PAGE_SIZE;
    loAddr = ROUNDUP(loAddr, align);
    for (i = 0; i < ARRAYSIZE(pools); i++) {
        PagePool *p = &pools[i];
        uint64 start = MAX(ROUNDUP(p->start, align), loAddr);
        uint64 end = start + len;
        if (p->len == 0 || end > hiAddr || end > p->start + p->len) {
            continue;
        }
        p->len -= end - p->start;
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
    return alloc_aligned_phys_pages(nPages, PAGE_SIZE);
}

PA
alloc_phys_page(void)
{
    return alloc_phys_pages(1);
}
