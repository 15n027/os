#include <string.h>
#include "smp/smp.h"
#include "x86/apic.h"
#include "memmap.h"
#include "x86/paging.h"
#include "x86/dt.h"
#include "smp/smpBootAP.h"
#include "x86/cpuid.h"

uint32 *cpuToApicMap;

static PA apTrampoline = INVALID_PA;
extern const char APTrampolineCodeStart[], APTrampolineCodeEnd[];
extern const char APTrampolineRealmodeBase[], APTrampolineEntry32[], APTrampolineEntry64[];

uint32 cpuToApic(uint32 cpu)
{
    return 0;
}

typedef struct {
    uint32 loadAddr;
    DTR32 gdtr;
    DTR64 idtr;
    uint64 gdt[4];
    uint32 cr3;
    FarPtr16 to32Bit;
    FarPtr32 to64Bit;
    void *stack;
    uint32 flags;
    void (*apStartFn)(void);
} APBootInfo;

static void
APCStart(void)
{
    Log("Hello from AP!!\n");
    HALT();
}

static void
InitBootTrampoline(void)
{
    APBootInfo *bi;
    FarPtr16 *base;
    uint32 patchOffset = (APTrampolineRealmodeBase - APTrampolineCodeStart);
    ASSERT_ON_COMPILE(offsetof(APBootInfo, loadAddr) == BOOTAP_BASE_OFF - BOOTAP_DATA_OFF);
    ASSERT_ON_COMPILE(offsetof(APBootInfo, gdtr) == BOOTAP_GDTR_OFF - BOOTAP_DATA_OFF);
    ASSERT_ON_COMPILE(offsetof(APBootInfo, idtr) == BOOTAP_IDTR_OFF - BOOTAP_DATA_OFF);
    ASSERT_ON_COMPILE(offsetof(APBootInfo, gdt) == BOOTAP_GDT_OFF - BOOTAP_DATA_OFF);
    ASSERT_ON_COMPILE(offsetof(APBootInfo, cr3) == BOOTAP_CR3_OFF - BOOTAP_DATA_OFF);
    ASSERT_ON_COMPILE(offsetof(APBootInfo, to32Bit) == BOOTAP_TO32_OFF - BOOTAP_DATA_OFF);
    ASSERT_ON_COMPILE(offsetof(APBootInfo, to64Bit) == BOOTAP_TO64_OFF - BOOTAP_DATA_OFF);
    ASSERT_ON_COMPILE(offsetof(APBootInfo, stack) == BOOTAP_STACK_OFF - BOOTAP_DATA_OFF);
    ASSERT_ON_COMPILE(offsetof(APBootInfo, flags) == BOOTAP_FLAGS_OFF - BOOTAP_DATA_OFF);
    ASSERT_ON_COMPILE(offsetof(APBootInfo, apStartFn) == BOOTAP_FUNC_OFF - BOOTAP_DATA_OFF);
    apTrampoline = alloc_aligned_phys_pages_in_range(0, 0x100000, 1, PAGE_SIZE);
    map_page(apTrampoline, apTrampoline, PT_RW | PT_P);
    PanicIf(apTrampoline == INVALID_PA, "Could not allocate AP trampoline low page");
    memcpy((void*)apTrampoline, APTrampolineCodeStart, APTrampolineCodeEnd - APTrampolineCodeStart);

    base = (void*)(apTrampoline + patchOffset);
    base->cs = apTrampoline >> 4;
    base->ip = patchOffset + sizeof *base;
    bi = (void*)(apTrampoline + 0x800);
    bi->loadAddr = apTrampoline;
    bi->flags = cpuid_isset(X2APIC);
    bi->gdtr.base = (VA)&bi->gdt[0];
    bi->gdtr.limit = sizeof bi->gdt - 1;
    asm("sidt %0" : "=m"(bi->idtr));

    ASSERT_ON_COMPILE(ARRAYSIZE(bi->gdt) >= 4);
    bi->gdt[0] = 0;
    bi->gdt[1] = DT_G | DT_L | DT_P | DT_DPL(0) |
        DT_TYPE(CS_NONCONFORMING_READABLE_NOTACCESSED) | DT_LIMIT(0xfffff);
    bi->gdt[2] = DT_G | DT_D | DT_P | DT_DPL(0) | DT_LIMIT(0xfffff) |
        DT_TYPE(DS_EXUP_WRITEABLE_NOTACCESSED);
    bi->gdt[3] = DT_G | DT_D | DT_P | DT_DPL(0) | DT_LIMIT(0xfffff) |
        DT_TYPE(CS_NONCONFORMING_READABLE_NOTACCESSED),

    bi->cr3 = GET_CR3();
    bi->to32Bit.cs = BOOTAP_CS32;
    bi->to32Bit.ip = apTrampoline + (APTrampolineEntry32 - APTrampolineCodeStart);
    bi->to64Bit.cs = BOOTAP_CS64;
    bi->to64Bit.ip = apTrampoline + (APTrampolineEntry64 - APTrampolineCodeStart);
    bi->apStartFn = APCStart;

    asm("xchgw %bx, %bx");
}

void BootAP(uint32 apicId)
{
    APBootInfo *bi;
    PA stackPhys;
    if (apTrampoline == INVALID_PA) {
        ASSERT(MyApicId() == 0);
        InitBootTrampoline();
    }
    bi = (void*)((uint8*)apTrampoline + BOOTAP_DATA_OFF);
    bi->stack = (void*)(alloc_va(get_kern_vma(), 2) + 2 * PAGE_SIZE);
    stackPhys = alloc_phys_pages(2);
    ASSERT(stackPhys != INVALID_PA);
    map_pages(stackPhys, (VA)bi->stack - 2 * PAGE_SIZE, 2, PT_NX | PT_RW | PT_P);
    DBG("apicId=%x trampoline=%lx", apicId, apTrampoline);
    ApicIPI(apicId,
            APIC_IPI_DELMODE_INIT |
            APIC_IPI_DESTMODE_PHYSICAL |
            APIC_IPI_DEST_APICID);
    for (uint32 i = 0; i < 2; i++) {
        DBG("");
        //       asm("hlt;hlt");
        ApicIPI(apicId,
                APIC_IPI_DELMODE_SIPI |
                APIC_IPI_DESTMODE_PHYSICAL |
                APIC_IPI_DEST_APICID);
    }
    DBG("");
    while ((bi->flags & 2) == 0) {
        asm("hlt" ::: "memory");
    }
    DBG("AP reports boot complete");
    HALT();
}
