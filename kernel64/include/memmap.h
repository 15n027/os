#pragma once
#include "basic_types.h"
#include "multiboot/multiboot.h"

typedef uint64 PFN;
#define INVALID_PFN (0x8000000000000000ull)

void map_page_local(VA va, PA pa, uint64 flags);
void vmm_init(void);
void vmm_earlyinit(void);
void pmm_init_multiboot(multiboot_info_t *mbi);

PA alloc_aligned_phys_pages_in_range(PA loAddr, PA hiAddr, size_t nPages, uintptr_t align);
PA alloc_aligned_phys_pages(size_t nPages, uintptr_t align);
PA alloc_phys_pages(size_t nPages);
PA alloc_phys_page(void);
PA alloc_low_phys_page(void);

PA vtophys(const void *v);

