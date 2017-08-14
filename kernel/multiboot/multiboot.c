#include "multiboot.h"

static struct multiboot_header __attribute__((section ("multiboot")))
__attribute__((used)) mbheader = {
    .magic = MULTIBOOT_HEADER_MAGIC,
    .flags = 0,
    .checksum = 0 - (MULTIBOOT_HEADER_MAGIC + 0)
};

