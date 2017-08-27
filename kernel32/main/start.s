.globl _start
.extern kern_entry
.extern _early_stack

check_cpuid:
        push $(1 << 21)         // Set cpuid bit
        popf
        pushf
        pop %eax
        test $(1 << 21), %eax
        jz .missing_cpuid
        push $0
        popf
        test $(1 << 21), %eax
        jz .missing_cpuid
        ret
.missing_cpuid:
        ud2
_start:
        cld
        mov $_early_stack, %esp
        push %ebx
        push %eax
        mov $((1 << 4) | (1 << 0)), %eax
        mov %eax, %cr0
        call check_cpuid
        xor %eax, %eax
        mov $_sbss, %edi
        mov $_bss_size, %ecx
        rep stosb
        call kern_entry
        ud2
