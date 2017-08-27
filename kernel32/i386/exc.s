.section .text


/*
  <iret stuff>
  <error code>
  <vector>
  <pusha>
*/
.align 1
.globl idt_common_asm


/* Far pointer for 64->32 transistion */
.long 0xffffffff
.long exc32Entry
.short 0x18

idt_common_asm:
.code64
        ljmp *-12(%rip)
.code32
exc32Entry:
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


