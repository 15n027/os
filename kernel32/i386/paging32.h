#pragma once
#include "x86/paging.h"
PA get_paging_root(void);
void init_4level_pagetable(void);
void map_pages_with_root(VA root, VA64 vBase, PA pBase, uint32 numPages, uint64 flags);
void map_page_with_root(VA root, VA64 vBase, PA pBase, uint64 flags);
void map_pages(VA vBase, PA pBase, uint32 numPages, uint64 flags);
void map_page(VA vBase, PA pBase, uint64 flags);


void enable_paging(VA root);
bool is_paging_enabled(void);
