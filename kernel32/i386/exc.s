.section .text

.extern idt_common


/*
  <iret stuff>
  <error code>
  <vector>
  <pusha>
*/
.align 1
.globl idt_common_asm
idt_common_asm:
     push %eax
     push %ebx
     push %ecx
     push %edx
     push %ebp
     push %esi
     push %edi
     push %esp /* IntrFrame *frame */
     call idt_common
     add $4, %esp
     pop %edi
     pop %esi
     pop %ebp
     pop %edx
     pop %ecx
     pop %ebx
     pop %eax
     add $8, %esp
     iret


