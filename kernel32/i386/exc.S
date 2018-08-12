#include "x86/exception.sinc"

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
     .irp r, eax, ebx, ecx, edx, ebp, esi, edi, esp
     push %\r
     .endr
     cld
     call idt_common
     add $4, %esp
     .irp r, edi, esi, ebp, edx, ecx, ebx, eax
     pop %\r
     .endr
     add $16, %esp
     ljmp *trampoline_32_64
do_iret:
.code64
     iretq

.macro gateStub vector
       .globl idt_entry_\vector
       idt_entry_\vector:
       .cfi_startproc
       .if \vector < 32
           .if ((1u << \vector) & EXC_HAS_ERRCODE) == 0
              push $0
           .endif
       .else
           push $0
       .endif
       push $\vector
       jmp idt_common_asm
       .cfi_endproc
.endm

.irpc i, 0123456789abcdef
.irpc j, 0123456789abcdef
      gateStub 0x\i\j
.endr
.endr

.section IDTData, "a", @progbits
.globl IDT
IDT:
         .fill 4096, 1, 0xaa


.section .rodata

trampoline_32_64:
.long do_iret
.short 0x8

trampoline_64_32:
.long exc32Entry
.short 0x18
