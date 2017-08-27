.extern kern_entry
.globl _start
.globl globalvar

.section .data
globalvar: .long 65
.section .bss
frob:
.space 0x1000

.section .bss.low
pml4:
.space 0x1000
pml3_lo:
.space 0x1000
pml3_hi:
.space 0x1000
pml2_lo:
.space 0x1000
pml2_hi:
.space 0x1000
.space 128
stack:
.section .text
start2:
        mov $stack, %rsp
        mov $pml3_lo, %rdi
        shlq $39, %rdi

        jmp _start

map4:
.section .text
_start:
        jmp kern_entry
