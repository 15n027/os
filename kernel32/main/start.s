.section .text

.globl _start
.extern kern_entry
.extern _stack_top

_start:
.cfi_startproc
        cld
        mov $_stack_top, %esp
        push %ebx
        push %eax
        mov $((1 << 4) | (1 << 0)), %eax
        mov %eax, %cr0
        xor %eax, %eax
        mov $_sbss, %edi
        mov $_bss_size, %ecx
        rep stosb
        call kern_entry
        ud2
.cfi_endproc
