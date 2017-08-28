.section .text


/*
  <iret stuff>
  <error code>
  <vector>
  <pusha>
*/

.globl idt_common_asm

.code64
.type idt_common_asm, @function
idt_common_asm:
     ljmp *trampoline_64_32(%rip)

.code32
.type exc32Entry, @function
exc32Entry:
     push %eax
     push %ebx
     push %ecx
     push %edx
     push %ebp
     push %esi
     push %edi
     push %esp /* IntrFrame *frame */
     cld
     call idt_common
     add $4, %esp
     pop %edi
     pop %esi
     pop %ebp
     pop %edx
     pop %ecx
     pop %ebx
     pop %eax
     add $16, %esp
     ljmp *trampoline_32_64
do_iret:
.code64
     iretq

.section .rodata

trampoline_32_64:
.long do_iret
.short 0x8

trampoline_64_32:
.long exc32Entry
.short 0x18
