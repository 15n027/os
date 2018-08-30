#pragma once
#include "basic_types.h"
#include "multiboot/multiboot.h"
#include "interrupt.h"
#include "x86/paging.h"
#include "smp/smp.h"

typedef uint64 PFN;
#define INVALID_PFN (0x8000000000000000ull)

#define SCRATCH_START PT_IDX_TO_VA(258, 0, 0, 0)
#define VASPACE_KERN0 PT_IDX_TO_VPN(259, 0, 0, 0)
#define VASPACE_ACPI PT_IDX_TO_VPN(260, 0, 0, 0)

void vmm_init(void);
void vmm_earlyinit(void);
void pmm_init_multiboot(multiboot_info_t *mbi);

PA alloc_aligned_phys_pages_in_range(PA loAddr, PA hiAddr, size_t nPages, uintptr_t align);
PA alloc_aligned_phys_pages(size_t nPages, uintptr_t align);
PA alloc_phys_pages(size_t nPages);
PA alloc_phys_page(void);
PA alloc_low_phys_page(void);

PA vtophys(const void *v);

#define VASPACE_HASH_BUCKETS 256

typedef enum {
    VM_AREA_USER,
    VM_AREA_OTHER,
    VM_AREA_HEAP,
    VM_AREA_ACPI,
    VM_AREA_MAX,
} vm_area_type;

typedef struct vma {
    VPN baseVPN;
    uint64 pages;
    struct vma *next, *prev;
} vma;

typedef struct vaspace {
    char name[64];
    VPN baseVPN;
    uint32 maxPages;
    uint32 usedPages;
    spinlock lock;
    vma alloc;
} vaspace;

typedef struct vaspace_node {
    struct vaspace_node *left, *right;
    vaspace val;
} vaspace_node;

void vaspace_register(vaspace_node *node, const char *name);
vaspace *vaspace_find(VPN vpn);
vaspace *vaspace_create(VPN baseVPN, uint32 maxPages, const char *name);

VA alloc_va(vaspace *vs, size_t n);
void dealloc_va(vaspace *vs, VA va, size_t n);
bool handle_pf(IretFrame *frame);

void map_pages(PA pa, VA va, uint32 n, uint64 flags);
void map_page(PA pa, VA va, uint64 flags);
void unmap_page(VA va);
void unmap_pages(VA va, unsigned n);
