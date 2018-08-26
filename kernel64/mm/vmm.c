#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "memmap.h"
#include "x86/x86_defs.h"
#include "x86/paging.h"
#include "kernel.h"
#include "percpu.h"
#include "katomic.h"

static bool vmm_inited;

static const va_range va_ranges[] = {
    [VM_AREA_OTHER] = {PT_IDX_TO_VA(384, 0, 0, 0), PT_IDX_TO_VA(385, 0, 0, 0)},
    [VM_AREA_HEAP] = {PT_IDX_TO_VA(260, 0, 0, 0), PT_IDX_TO_VA(261, 0, 0, 0)},
    [VM_AREA_MGMT] = {PT_IDX_TO_VA(261, 0, 0, 0), PT_IDX_TO_VA(261, 0, 1, 0)},
    [VM_AREA_USER] = {PT_IDX_TO_VA(0, 0, 0, 256), PT_IDX_TO_VA(128, 0, 0, 0)},
    [VM_AREA_ACPI] = {PT_IDX_TO_VA(0, 511, 511, 0), PT_IDX_TO_VA(0, 511, 511, 511)},
};

static vma kern_vma;

vma *
get_kern_vma(void)
{
    return &kern_vma;
}

VA
alloc_va_from(vma *vm, vm_area_type type, size_t n)
{
    VA ret;
    va_range *r;
    vma_range *u;
    ASSERT(type >= 0 && type != VM_AREA_MGMT && type < VM_AREA_MAX);
    if (vm->used_cnt + 1 >= (vm->free[VM_AREA_MGMT].end - vm->free[VM_AREA_MGMT].start) / sizeof *vm->used) {
        printf("used_cnt=%u\n", vm->used_cnt);
        printf("start=%lx end=%lx\n", vm->free[VM_AREA_MGMT].start, vm->free[VM_AREA_MGMT].start);
    }
    ASSERT(vm->used_cnt + 1 < (vm->free[VM_AREA_MGMT].end - vm->free[VM_AREA_MGMT].start) / sizeof *vm->used);
    r = &vm->free[type];
    if (r->end - r->start < n) {
        return 0;
    }
    ret = r->start;
    r->start += n * PAGE_SIZE;
    u = &vm->used[vm->used_cnt];
    vm->used_cnt++;
    u->start = ret;
    u->end = ret + n * PAGE_SIZE;
    return ret;
}

VA
alloc_va(vma *vm, size_t n)
{
    return alloc_va_from(vm, VM_AREA_OTHER, n);
}

PA
vtophys(const void *p)
{
    PA ret;
    uint32 i;
    PTE pte;
    uint64 off = 0;

    for (i = 4; i > 0; i--) {
        off = off * 512 + PML_OFF((VA)p, i);
        pte = PML(i)[off];
        if ((pte & PT_P) == 0) {
            //         DBG("%p NP", p);
            // ASSERT(0);
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

void
vmm_earlyinit(void)
{
    // Clear 32 bit kernel entries
    for (uint32 i = 1; i < 256; i++) {
        PML4[i] = 0;
    }
    for (uint32 lvl = 3; lvl > 0; lvl--) {
        for (uint32 i = 0; i < 512; i++) {
            if (i != PML_OFF(0xb8000, lvl)) {
                PML(lvl)[i] = 0;
            }
        }
    }
    invtlb();
}

void
vmm_init(void)
{
    memcpy(kern_vma.free, va_ranges, sizeof va_ranges);
    kern_vma.used = (void*)va_ranges[VM_AREA_MGMT].start;
    kern_vma.used_cnt = 0;
    printf("VA ranges:\n");
    for (size_t i = 0; i < ARRAYSIZE(kern_vma.free); i++) {
        printf("[%016lx - %016lx]\n", kern_vma.free[i].start, kern_vma.free[i].end);
    }
    vmm_inited = TRUE;
}

static void
unmap_page_int(VA va)
{
    PTE *pte;
    int lvl;
    uint64 off = 0;
    for (lvl = 4; lvl > 0; lvl--) {
        off = off * 512 + PML_OFF(va, lvl);
        pte = &PML(lvl)[off];
        if ((*pte & PT_P) == 0) {
            break;
        }
    }
    //    DBG("*pte = %lx", *pte);
    Atomic_Write64(pte, 0);
}

void
unmap_pages(VA va, uint32 n)
{
    for (uint32 i = 0; i < n; i++) {
        unmap_page_int(va);
    }
    invtlb();
}

void
unmap_page(VA va)
{
    unmap_page_int(va);
    invpage((void*)va);
}

static void
zero_physpage(PA pa)
{
    PerCpu *perCpu = GetPerCpu();
    uint32 idx;
    VA va;
    //    DBG("percpu=%p start=%lx used=%x", perCpu, perCpu->scratchStart, perCpu->scratchUsed);
    ASSERT(perCpu->scratchUsed < 512);
    idx = Atomic_ReadInc32(&perCpu->scratchUsed);
    va = perCpu->scratchStart + idx * PAGE_SIZE;
    map_page(pa, va, PT_P | PT_RW);
    memset((void*)va, 0, PAGE_SIZE);
    unmap_page(va);
    Atomic_ReadDec32(&perCpu->scratchUsed);
}

static void
map_page_int(PA pa, VA va, uint64 flags)
{
    uint32 lvl;
    PTE *pte;
    uint64 off = 0;

    for (lvl = 4; lvl > 1; lvl--) {
        off = (off << PAE_4LEVEL_INDEX_BITS) + PML_OFF(va, lvl);
        pte = &PML(lvl)[off];
        //        DBG("lvl %u pte=%08lx", lvl, *pte);
        if ((*pte & PT_P) == 0) {
            PA pa = alloc_phys_page();
            if (LIKELY(vmm_inited)) {
                zero_physpage(pa);
                Atomic_Write64(pte, PT_ADDR_4K(pa) | PT_P | PT_RW);
            } else {
                uint64 nextOffset = (off << PAE_4LEVEL_INDEX_BITS) + PML_OFF(0, lvl - 1);
                DBG("early mapping %lx -> %lx", pa, va);
                Atomic_Write64(pte, PT_ADDR_4K(pa) | PT_P | PT_RW);
                memset(&PML(lvl - 1)[nextOffset], 0, PAGE_SIZE);
                invtlb();
            }
        }
    }

    Atomic_Write64(&PML1[off * 512 + PML1_OFF(va)], PT_ADDR_4K(pa) | (flags & PT_FLAGS));
}

void
map_pages(PA pa, VA va, uint32 n, uint64 flags)
{
    for (uint32 i = 0; i < n; i++) {
        map_page_int(pa, va, flags);
        pa += PAGE_SIZE;
        va += PAGE_SIZE;
    }
    invtlb();
}
void
map_page(PA pa, VA va, uint64 flags)
{
    map_page_int(pa, va, flags);
    invpage((void*)va);
}

bool
handle_pf(IretFrame *unused)
{
    PA pa = INVALID_PA;
    uint32 i;
    VA addr = GET_CR2();
    vma *vm = get_kern_vma();
    //    HALT();
    if (addr >= va_ranges[VM_AREA_MGMT].start && addr < va_ranges[VM_AREA_MGMT].end) {
        ASSERT(addr > 0xffff800000000000ull);
        size_t used_sz = vm->used_cnt * sizeof *vm->used;
        if (va_ranges[VM_AREA_MGMT].start + used_sz < addr) {
            return false;
        }
        pa = alloc_phys_page();
        DBG("alloc mgmt page: va=%lx pa=%lx", addr, pa);
    } else {
        for (i = 0; i < vm->used_cnt; i++) {
            if (addr >= vm->used[i].start && addr < vm->used[i].end) {
                ASSERT(addr > 0xffff800000000000ull);
                pa = alloc_phys_page();
                break;
            }
        }
    }
    if (pa == INVALID_PA) {
        return false;
    }
    map_page(pa, addr & ~PAGE_MASK, PT_RW | PT_NX | PT_P);
    return true;
}
