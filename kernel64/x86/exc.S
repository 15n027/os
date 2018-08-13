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

idt_common_asm:
    push %r15
    push %r14
    push %r13
    push %r12
    push %r11
    push %r10
    push %r9
    push %r8
    push %rdi
    push %rsi
    push %rbp
    push %rbx
    push %rdx
    push %rcx
    push %rax
    mov %rsp, %rdi
 /* IntrFrame *frame */
    cld
    call idt_common
    mov %rax, %rsp
    pop %rax
    pop %rcx
    pop %rdx
    pop %rbx
    pop %rbp
    pop %rsi
    pop %rdi
    pop %r8
    pop %r9
    pop %r10
    pop %r11
    pop %r12
    pop %r13
    pop %r14
    pop %r15
    add $16, %rsp
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

