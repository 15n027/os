#include <stdio.h>
#include <string.h>
#include "kassert.h"
#include "loader.h"
#include "basic_types.h"
#include "pagealloc.h"
#include "i386/paging32.h"
#include "elf.h"


uint64
load_module(const char *name, const void *image, size_t len)
{
    const uint8 *const base = image;
    const Elf64_Ehdr *const ehdr = image;
    static const VA SCRATCH_VA = 3 * GB;

    printf("[ %p - %p ] '%s'\n", base, image + len, name);
    ASSERT(!memcmp(ehdr->e_ident, ELFMAG, SELFMAG));
    ASSERT(ehdr->e_type == ET_EXEC);
    ASSERT(ehdr->e_machine == EM_X86_64);
    printf("Entry: %llx\n", ehdr->e_entry);
    printf("phoff=%llx num=%u\n", ehdr->e_phoff, ehdr->e_phnum);
    ASSERT(ehdr->e_phnum != PN_XNUM);
    printf("shoff=%llx\n", ehdr->e_shoff);
    for (Elf64_Half i = 0; i < ehdr->e_phnum; i++) {
        const Elf64_Phdr *phdr = (const Elf64_Phdr*)(base + ehdr->e_phoff + i * sizeof *phdr);
        VA64 start = phdr->p_vaddr, end = phdr->p_vaddr + phdr->p_memsz;
        PA pages;
        uint32 numPages = PAGES_SPANNED(start, end);
        uint64 perms = PT_P;

        if (phdr->p_type != PT_LOAD) {
            continue;
        }
        printf("Module %s[%u] requires %u pages from [ %llx - %llx ]\n", name, i,
                numPages, start, end);
        ASSERT(numPages < 3 * GB / PAGE_SIZE);
        pages = alloc_aligned_phys_pages_in_range(4 * GB, ~0ull, numPages, PAGE_SIZE);
        if (pages == INVALID_PA) {
            pages = alloc_phys_pages(numPages);
        }
        ASSERT(pages != INVALID_PA);
        if ((phdr->p_flags & PF_X) == 0) {
            perms |= PT_NX;
        }
        if (phdr->p_flags & PF_W) {
            perms |= PT_RW;
        }
        map_pages_with_root(get_paging_root(), SCRATCH_VA, pages, numPages,
                PT_P | PT_RW | PT_NX);
        map_pages_with_root(get_paging_root(), start, pages, numPages, perms);
        memset(PA_TO_PTR(SCRATCH_VA), 0, phdr->p_memsz);
        memcpy(PA_TO_PTR(SCRATCH_VA), base + phdr->p_offset, phdr->p_filesz);
    }
    return ehdr->e_entry;
}
