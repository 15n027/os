.extern kern_entry
.globl _start
.extern _stack0_top

.section .text
_start:
    .cfi_startproc
        lea (_stack0_top - 128)(%rip), %rsp
        sub $10, %rsp
        lea IDT(%rip), %rax
        movw $4095, 0(%rsp)
        mov %rax, 2(%rsp)
        lidt (%rsp)
        add $10, %rsp
        cld
        call kern_entry
        ud2
    .cfi_endproc
