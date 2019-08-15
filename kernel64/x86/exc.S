#include "x86/exception.sinc"

.section .text

.code64
.cfi_sections .debug_frame

.globl idt_common_asm
.type idt_common_asm, @function
.extern idt_common

idt_entry:
    .cfi_startproc
    .cfi_def_cfa_offset 0
    .cfi_rel_offset rip, 0x8
    .cfi_rel_offset rsp, 0x20
    push %rdx
    .cfi_adjust_cfa_offset 8

.idt_with_errcode:
    cld

    // 5 (IretFrame) + ax, dx (2) + cx,di,si,8,9,10,11 (7) = 16
    // Therefore stack is aligned to 16 bytes at the call
    .irp r, cx, di, si, 8, 9, 10, 11
    push %r\r
    .cfi_adjust_cfa_offset 8
    .endr
    lea 0x48(%rsp), %rdi
    movzx %al, %esi
    call idt_common

    .irp r, 11, 10, 9, 8, si, di, cx, ax, dx
    pop %r\r
    .cfi_adjust_cfa_offset -8
    .endr
    iretq
    .cfi_endproc

.macro gateStub vector
       .globl idt_entry_\vector
       idt_entry_\vector:
       .cfi_startproc
       .cfi_def_cfa_offset 0
       .if \vector < 32
          .cfi_rel_offset rip, 0x8
          .cfi_rel_offset rsp, 0x20
           .if ((1u << \vector) & EXC_HAS_ERRCODE) != 0
              xchg (%rsp), %rdx
           .endif
       .else
          .cfi_rel_offset rip, 0x0
          .cfi_rel_offset rsp, 0x18
       .endif
       push %rax
       .cfi_adjust_cfa_offset 8
       mov $\vector, %al
       .if \vector < 32
           .if ((1u << \vector) & EXC_HAS_ERRCODE) != 0
               jmp .idt_with_errcode
           .else
              jmp idt_entry
           .endif
       .else
           jmp idt_entry
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

