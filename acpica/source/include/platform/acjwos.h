#pragma once
#include "basic_types.h"
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

typedef bool BOOLEAN;
typedef int8 INT8;
typedef uint8 UINT8;
typedef int16 INT16;
typedef uint16 UINT16;
typedef int32 INT32;
typedef uint32 UINT32;
typedef int64 INT64;
typedef uint64 UINT64;

#define ACPI_SPINLOCK unsigned
#define ACPI_SEMAPHORE unsigned

#define ACPI_MUTEX_TYPE ACPI_BINARY_SEMAPHORE
#define ACPI_USE_LOCAL_CACHE
#define ACPI_USE_SYSTEM_CLIBRARY
#define ACPI_USE_DO_WHILE_0
#define ACPI_IGNORE_PACKAGE_RESOLUTION_ERRORS

#define ACPI_USE_SYSTEM_INTTYPES
#define ACPI_USE_GPE_POLLING

#define ACPI_USE_NATIVE_MATH64



#define ACPI_MACHINE_WIDTH          64
#define COMPILER_DEPENDENT_INT64    long
#define COMPILER_DEPENDENT_UINT64   unsigned long
#undef ACPI_DEBUGGER
