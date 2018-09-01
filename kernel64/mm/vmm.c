#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "memmap.h"
#include "x86/x86_defs.h"
#include "x86/paging.h"
#include "kernel.h"
#include "percpu.h"
#include "katomic.h"
#include "x86/msr.h"

static bool vmm_inited;

static vaspace_node vaspace_root = {
    .val.baseVPN = VASPACE_KERN0,
    .val.maxPages = GB / PAGE_SIZE,
    .val.alloc.baseVPN = VASPACE_KERN0,
    .val.alloc.prev = &vaspace_root.val.alloc,
    .val.alloc.next = &vaspace_root.val.alloc,
};

vaspace *
vaspace_find(VPN vpn)
{
    vaspace_node *tree = &vaspace_root;
    while (tree != NULL) {
        if (vpn < tree->val.baseVPN) {
            tree = tree->left;
        } else if (vpn >= tree->val.baseVPN &&
                   vpn <= tree->val.baseVPN + tree->val.maxPages) {
            return &tree->val;
        } else {
            tree = tree->right;
        }
    }
    return NULL;
}

void
vaspace_register(vaspace_node *node, const char *name)
{
    vaspace_node *tree = &vaspace_root;
    ASSERT(!vaspace_find(node->val.baseVPN));
    ASSERT(!vaspace_find(node->val.baseVPN + node->val.maxPages * PAGE_SIZE));
    strncpy(node->val.name, name, sizeof node->val.name);
    node->val.name[sizeof node->val.name - 1] = 0;
    while (tree != NULL) {
        if (node->val.baseVPN < tree->val.baseVPN) {
            if (tree->left == NULL) {
                tree->left = node;
                return;
            } else {
                tree = tree->left;
            }
        } else {
            if (tree->right == NULL) {
                tree->right = node;
                return;
            } else {
                tree = tree->right;
            }
        }
    }
    NOT_REACHED();
}

vaspace *
vaspace_create(VPN baseVPN, uint32 maxPages, const char *name)
{
    vaspace_node *vs;
    ASSERT(IMPLIES(baseVPN != 0, !vaspace_find(baseVPN) &&
                                 !vaspace_find(baseVPN + maxPages)));
    ASSERT(maxPages > 0);
    vs = malloc(sizeof *vs);
    if (vs == NULL) {
        return NULL;
    }
    memset(vs, 0, sizeof *vs);
    vs->val.alloc.baseVPN = vs->val.baseVPN = baseVPN;
    vs->val.alloc.prev = vs->val.alloc.next = &vs->val.alloc;
    if (baseVPN == 0) {
        DBG("attempt dynamic alloc vaspace");
        NOT_REACHED();
    }
    vs->val.maxPages = maxPages;
    vaspace_register(vs, name);
    return &vs->val;
}

void
dealloc_va(vaspace *vs, VA va, size_t n)
{
    vma *v;
    VPN vpn = VA_TO_VPN(va);
    ASSERT(n > 0);
    if (vs == NULL) {
        vs = &vaspace_root.val;
    }
    //    DBG("dealloc %s 0x%lx n=%zu", vs->name, va, n);
    v = &vs->alloc;
    spin_lock(&vs->lock);
    do {
        if (vpn >= v->baseVPN) {
            if (vpn + n == v->baseVPN + v->pages) {
                v->pages -= n;
                if (v->pages == 0 && v != &vs->alloc) {
                    v->prev->next = v->next;
                    v->next->prev = v->prev;
                    memset(v, 0, sizeof *v);
                    free(v);
                }
            } else {
                NOT_REACHED();
            }
        }
        v = v->next;
    } while (v != &vs->alloc);
    spin_unlock(&vs->lock);
}

VA
alloc_va(vaspace *va, size_t n)
{
    VA ret = 0;
    vma *tail;
    vma *node;

    if (va == NULL) {
        va = &vaspace_root.val;
    }
    spin_lock(&va->lock);
    tail = va->alloc.prev;
    if (va->usedPages + n > va->maxPages) {
        ret = 0;
        ASSERT(0);
        goto out;
    }
    if (va->baseVPN + va->maxPages >= tail->baseVPN + n) {
        va->usedPages += n;
        ret = (tail->baseVPN + tail->pages) << PAGE_SHIFT;
        tail->pages += n;
        goto out;
    }
    for (node = &va->alloc; node != tail; node = node->next) {
        if (node->baseVPN + node->pages + n < node->next->baseVPN) {
            va->usedPages += n;
            ret = (node->baseVPN + node->pages) << PAGE_SHIFT;
            node->pages += n;
            goto out;
        }
    }
    ASSERT(0);

out:
    spin_unlock(&va->lock);
    //    DBG("alloc_va: %s ret=%lx", va->name, ret);
    return ret;
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
unmap_pages(VA va, unsigned n)
{
    unsigned i;
    //DBG("unmap: %lx - %lx", va, va * n * PAGE_SIZE);
    if (n < 8) {
        for (i = 0; i < n; i++) {
            unmap_page(va + i * PAGE_SIZE);
        }
    } else {
        for (i = 0; i < n; i++) {
            unmap_page_int(va);
        }
        invtlb();
    }
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
    PerCpu *perCpu;
    uint32 idx;
    VA va;
    DBG("");
    DBG("gsmsr: 0x%lx", RDMSR(IA32_GS_BASE));
    fflush(0);
    perCpu = GetPerCpu();
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

    ASSERT(IMPLIES(va == 0, (flags & PT_P) == 0));
    for (lvl = 4; lvl > 1; lvl--) {
        off = (off << PAE_4LEVEL_INDEX_BITS) + PML_OFF(va, lvl);
        pte = &PML(lvl)[off];
        //        DBG("lvl %u pte=%08lx", lvl, *pte);
        if ((*pte & PT_P) == 0) {
            PA pa = alloc_phys_page();
            if (vmm_inited) {
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
    PA pa;
    VA addr = GET_CR2();

    if (addr < 0x4000) {
        DBG("Zero page access");
        return false;
    }
    pa = alloc_phys_page();
    //    DBG("pa=%lx", pa);
    if (pa == INVALID_PA) {
        return false;
    }
    map_page(pa, addr & ~PAGE_MASK, PT_RW | PT_NX | PT_P);
    return true;
}
