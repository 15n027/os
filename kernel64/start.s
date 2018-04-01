.extern kern_entry
.globl _start
.extern _stack0_top

.section .text
_start:
        movabs $_stack0_top, %rsp
        mov %rsp, %rbp
        cld
        call kern_entry
        int3
