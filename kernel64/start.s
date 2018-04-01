.extern kern_entry
.globl _start
.extern _stack

.section .text
_start:
        movabs $_stack, %rsp
        mov %rsp, %rbp
        cld
        call kern_entry
        int3
