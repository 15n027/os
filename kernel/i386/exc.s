.section .text

.globl idt_base
.extern idt_common

.macro idt from=0, to=64
    .if \from == 8 || \from == 10 || \from == 11 || \from == 12 || \from == 13 || \from == 14 || \from == 17
        call idt_common_asm
        int3
        int3
    .else
        push $0
        call idt_common_asm
    .endif
    int3
    .if \to-\from
        idt "(\from+1)",\to
    .endif
.endm

.align 8
idt_base:
idt 0, 64
idt 65, 128
idt 129, 192
idt 193, 255

/*
  <iret stuff>
  <error code>
  <idt_base + vector * 8 + 7>
  <pusha>
*/
.align 1
idt_common_asm:
     push %eax
     push %ebx
     push %ecx
     push %edx
     push %ebp
     push %esi
     push %edi
     call idt_common
     pop %edi
     pop %esi
     pop %ebp
     pop %edx
     pop %ecx
     pop %ebx
     pop %eax
     add $8, %esp
     iret


