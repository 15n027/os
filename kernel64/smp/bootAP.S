#include "smpBootAP.h"


.section .rodata
.globl APTrampolineCodeStart, APTrampolineCodeEnd
.globl APTrampolineRealmodeBase, APTrampolineEntry32, APTrampolineEntry64

APTrampolineCodeStart:
    .code16
    .byte 0xea
APTrampolineRealmodeBase:
    .long 0
    movl %cs:(BOOTAP_BASE_OFF), %edi // cs.base
    lgdt %cs:(BOOTAP_GDTR_OFF)
    movl %cs:(BOOTAP_CR3_OFF), %ebx // cr3

    mov $((1 << 4) | (1 << 0)), %ax
    mov %eax, %cr0
    ljmp *%cs:(BOOTAP_TO32_OFF)

APTrampolineEntry32:
    .code32
    mov %ebx, %cr3

    mov $0xc0000080, %ecx // efer
    xor %edx, %edx
    mov $((1<< 0) | (1 << 8) | (1 << 10) | (1 << 11)), %eax
    wrmsr
    mov %cr4, %eax
    or $(1 << 5), %eax
    mov %eax, %cr4  // pae
    mov %cr0, %eax
    or $0x80010000, %eax
    mov %eax, %cr0  // cr0.pg
    ljmp *%cs:BOOTAP_TO64_OFF(%edi)

APTrampolineEntry64:
    .code64
    mov %edi, %edi
    lidt BOOTAP_IDTR_OFF(%rdi)
    mov $BOOTAP_DS, %ebx

    mov %ebx, %ds
    mov %ebx, %es
    mov %ebx, %ss
    mov BOOTAP_STACK_OFF(%rdi), %rsp
    xor %ebx, %ebx
    mov %ebx, %fs
    mov %ebx, %gs
    mov BOOTAP_FUNC_OFF(%rdi), %rsi
    lock
    or $2, BOOTAP_FLAGS_OFF(%rdi)
    mfence
    jmp *%rsi

APTrampolineCodeEnd:
