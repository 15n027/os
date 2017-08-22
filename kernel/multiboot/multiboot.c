#include "multiboot.h"

#define FLAGS (MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO | MULTIBOOT_VIDEO_MODE)
static struct multiboot_header __attribute__((section (".multiboot")))
__attribute__((used)) mbheader = {
    .magic = MULTIBOOT_HEADER_MAGIC,
    .flags = FLAGS,
    .checksum = -(MULTIBOOT_HEADER_MAGIC + FLAGS),
    .mode_type = 1, /* Text mode */
    .width = 80,
    .height = 25,
    .height = 0,

};

