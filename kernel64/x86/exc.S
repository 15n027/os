#include "x86/exception.sinc"

.section .text

.code64

/*
  <iret stuff>
  <error code>
  <vector>
  <pusha>
*/

.globl idt_common_asm
.type idt_common_asm, @function
.extern idt_common

idt_no_errcode:
    push %rdx
idt_with_errcode:
    cld
    .irp r, cx, di, si, 8, 9, 10, 11
    push %r\r
    .endr
    lea 0x40(%rsp), %rdi
    movzx %al, %esi
    call idt_common
    .irp r, 11, 10, 9, 8, si, di, cx, ax, dx
    pop %r\r
    .endr
    iretq

.macro gateStub vector
       .globl idt_entry_\vector
       idt_entry_\vector:
       .cfi_startproc
       .if \vector < 32
           .if ((1u << \vector) & EXC_HAS_ERRCODE) != 0
              xchg (%rsp), %rdx
           .endif
       .endif
       push %rax
       mov $\vector, %al
       .if \vector < 32
           .if ((1u << \vector) & EXC_HAS_ERRCODE) != 0
               jmp idt_with_errcode
           .else
              jmp idt_no_errcode
           .endif
       .else
           jmp idt_no_errcode
       .endif
       .cfi_endproc
.endm

.section IDTStubs, "ax", @progbits
.irpc i, 0123456789abcdef
.irpc j, 0123456789abcdef
      gateStub 0x\i\j
.endr
.endr

.section IDTData, "a", @progbits
.globl IDT
IDT:
         .fill 4096, 1, 0xaa

