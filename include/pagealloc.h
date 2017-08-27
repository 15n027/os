#pragma once
#include "basic_types.h"

typedef struct {
    uint64 start;
    uint64 len;
} PagePool;
void print_phys_map(void);
void complete_phys_map(void);
void mark_phys_range(PA start, uint64 len, bool available);
PA alloc_aligned_phys_pages_in_range(PA loAddr, PA hiAddr, size_t nPages,
        uintptr_t align);
PA alloc_aligned_phys_pages(size_t nPages, uintptr_t align);
PA alloc_phys_pages(size_t nPages);
PA alloc_phys_page(void);

PA alloc_virt_pages(size_t nPages, uintptr_t align);
