.globl _start
.extern kern_entry
.extern early_stack
.extern _sctors
.extern _ectors

_start:
        mov $early_stack, %esp
        push %ebx
        push %eax
        push $0
        popf
        xor %eax, %eax
        mov $_sbss, %edi
        mov $_bss_size, %ecx
        rep stosb
        call kern_entry
        int3
