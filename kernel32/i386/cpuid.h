#pragma once
#include <stdint.h>
#include "regname.h"

#define CPUID(name, eax, ecx, reg, bit, length) \
    name,
enum CpuidItem {
#include "cpuid-entry.h"
CPUID_ITEM_MAX
};

#undef CPUID

typedef struct {
    uint32_t reg[4];
} CpuidResult;

const char *cpuid_item_to_name(enum CpuidItem item);
void cpuid(uint32_t eaxIn, uint32_t ecxIn, CpuidResult *info);
bool cpuid_isset(enum CpuidItem item);
uint32_t cpuid_get(enum CpuidItem item);
