#include <stdint.h>
#include <stdbool.h>
#include "basic_defs.h"
#include "kassert.h"
#include "cpuid.h"

typedef struct {
    const char *name;
    uint32_t eax;
    uint32_t ecx;
    enum RegName reg;
    uint8_t start;
    uint8_t length;
} CpuidDescriptor;


#define CPUID(name, eax_, ecx_, reg_, start_, length_)  \
    [name] = {#name, eax_, ecx_, reg_, start_, length_},


static const CpuidDescriptor cpuidLookupInfo[] = {
#include "cpuid-entry.h"
};
#undef CPUID

void
cpuid(uint32_t eaxIn, uint32_t ecxIn, CpuidResult *info)
{
    info->reg[EAX] = eaxIn;
    info->reg[ECX] = ecxIn;
    asm("cpuid"
        : "+a"(info->reg[EAX]), "=b"(info->reg[EBX]), "+c"(info->reg[ECX]), "=d"(info->reg[EDX])
        :
        : "cc");
}

const char *
cpuid_item_to_name(enum CpuidItem item)
{
    ASSERT(item < ARRAYSIZE(cpuidLookupInfo));
    return cpuidLookupInfo[item].name;
}

bool
cpuid_isset(enum CpuidItem item)
{
    uint32_t ret = cpuid_get(item);
    ASSERT(ret <= 1);
    return ret;
}

uint32_t
cpuid_get(enum CpuidItem item)
{
    CpuidResult r;
    const CpuidDescriptor *d;
    ASSERT(item < ARRAYSIZE(cpuidLookupInfo));
    d = &cpuidLookupInfo[item];
    cpuid(d->eax, d->ecx, &r);
    ASSERT(d->reg < ARRAYSIZE(r.reg));
    return (r.reg[d->reg] >> d->start) & ((1ull << d->length) - 1);
}
