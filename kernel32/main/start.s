.section .text

.globl _start
.extern kern_entry
.extern _stack_top

_start:
.cfi_startproc
        cld
        lea _stack_top, %esp
        push %ebx
        push %eax
        mov $((1 << 16) | (1 << 4) | (1 << 0)), %eax
        mov %eax, %cr0

        sub $6, %esp
        movw $0xfff, (%esp)
        lea IDT, %eax
        mov %eax, 2(%esp)
        lidt (%esp)
        add $6, %esp

        xor %eax, %eax
        lea _sbss, %edi
        lea _bss_size, %ecx
        rep stosb
        call kern_entry
        ud2
.cfi_endproc
