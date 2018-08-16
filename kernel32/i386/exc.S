#include "x86/exception.sinc"

.section .text

.code64
idt_common_asm_no_err:
    .cfi_startproc
    xor %ebx, %ebx
idt_common_asm_with_err:
    .irp r, cx, dx, bp, si, di
     push %r\r
     .endr
     movzx %al, %eax
     ljmp *trampoline_64_32(%rip)
     .cfi_endproc

.code32
.type exc32Entry, @function
exc32Entry:
     .cfi_startproc
     cld
     push %ebx
     push %eax
     sub $4, %esp
     lea 68(%esp), %eax
     mov %eax, (%esp)
     call idt_common
     add $12, %esp
     ljmp *trampoline_32_64
     .cfi_endproc
do_iret:
.code64
     .cfi_startproc
     .irp r, di, si, bp, dx, cx, bx, ax
     pop %r\r
     .endr

     iretq
     .cfi_endproc

.macro gateStub vector
       .globl idt_entry_\vector
       idt_entry_\vector:
       .cfi_startproc
       .if \vector < 32
           .if ((1u << \vector) & EXC_HAS_ERRCODE) != 0
              xchg (%rsp), %rbx
           .else
              push %rbx
           .endif
       .else
           push %rbx
       .endif
       push %rax
       mov $\vector, %al
       .if \vector < 32
           .if ((1u << \vector) & EXC_HAS_ERRCODE) != 0
              jmp idt_common_asm_with_err
           .else
              jmp idt_common_asm_no_err
           .endif
       .else
           jmp idt_common_asm_no_err
       .endif
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
