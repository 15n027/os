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

typedef enum {
    VM_AREA_USER,
    VM_AREA_OTHER,
    VM_AREA_HEAP,
    VM_AREA_MGMT,
    VM_AREA_ACPI,
    VM_AREA_MAX,
} vm_area_type;

typedef struct va_range {
    VPN start, end;
} va_range;

typedef struct vma_range {
    VPN start, end;
    struct vma_range *next;
} vma_range;

typedef struct vma {
    va_range free[VM_AREA_MAX];
    vma_range *used;
    unsigned used_cnt;
} vma;

vma *get_kern_vma(void);
VA alloc_va_from(vma *vm, vm_area_type type, size_t n);
VA alloc_va(vma *vm, size_t n);
bool handle_pf(VA rip, unsigned err, VA addr);

void map_pages(PA pa, VA va, uint32 n);
void map_page(PA pa, VA va);
void unmap_page(VA va);
void unmap_pages(VA va, uint32 n);
