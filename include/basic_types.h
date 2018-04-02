#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "kassert.h"
#include "basic_defs.h"
#define TYPE(x) typedef x##_t x

TYPE(uint8);
TYPE(uint16);
TYPE(uint32);
TYPE(uint64);

TYPE(int8);
TYPE(int16);
TYPE(int32);
TYPE(int64);

typedef uintptr_t VA;
typedef uintptr_t VPN;

typedef uint64 PA;
typedef uint64 PFN;
typedef uint64 VA64;

#define INVALID_PA (0xffffffffffffbeefull)
#define PTR_TO_VA(ptr) ((VA)(ptr))
#define PTR_TO_PA(ptr) ((PA)(uintptr_t)(ptr))
#if ARCH_BITS == 32
#define PA_TO_PTR(pa) ({ASSERT((pa) < 4 * GB); ((void*)(uintptr_t)(pa));})
#else
#define PA_TO_PTR(pa) ((void*)(uintptr_t)(pa))
#endif

#define VA_TO_PTR(va) ((void*)(uintptr_t)(va))

#define TRUE true
#define FALSE false

#undef TYPE
