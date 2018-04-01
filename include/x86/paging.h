#pragma once
#include "basic_types.h"

#define LA_ADDR_BITS 64
#define PAE_4LEVEL_INDEX_BITS 9
#define PML_SHIFT(level) (((level) - 1) * PAE_4LEVEL_INDEX_BITS + PAGE_SHIFT)
#define PML_INDEX_MASK ((1ull << PAE_4LEVEL_INDEX_BITS) - 1)
#define PML_OFF(la, level) (((la) >> PML_SHIFT(level)) & PML_INDEX_MASK)

#define PML4_OFF(la) PML_OFF(PTR_TO_PA(la), 4)
#define PML3_OFF(la) PML_OFF(PTR_TO_PA(la), 3)
#define PML2_OFF(la) PML_OFF(PTR_TO_PA(la), 2)
#define PML1_OFF(la) PML_OFF(PTR_TO_PA(la), 1)

#define PT_NX_SHIFT  63
#define PT_G_SHIFT   8
#define PT_PS_SHIFT  7
#define PT_D_SHIFT   6
#define PT_A_SHIFT   5
#define PT_PCD_SHIFT 4
#define PT_PWT_SHIFT 3
#define PT_US_SHIFT  2
#define PT_RW_SHIFT  1
#define PT_P_SHIFT   0

#define PT_NX  (1ull << PT_NX_SHIFT)
#define PT_G   (1ull << PT_G_SHIFT)
#define PT_PS  (1ull << PT_PS_SHIFT)
#define PT_D   (1ull << PT_D_SHIFT)
#define PT_A   (1ull << PT_A_SHIFT)
#define PT_PCD (1ull << PT_PCD_SHIFT)
#define PT_PWT (1ull << PT_PWT_SHIFT)
#define PT_US  (1ull << PT_US_SHIFT)
#define PT_RW  (1ull << PT_RW_SHIFT)
#define PT_P   (1ull << PT_P_SHIFT)

#define PT_ENTRY_TO_PA(entry) pt_entry_to_pa(entry)

#define PT_ADDR_4K(pa) ((pa) & ~PAGE_MASK)
#define PT_ADDR_2M(pa) ((pa) & ~LPAGE_MASK)

typedef uint64 PTE;

PA pt_entry_to_pa(PTE entry);
