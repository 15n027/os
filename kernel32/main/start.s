.globl _start
.extern kern_entry
.extern early_stack
.extern _sctors
.extern _ectors

run_global_ctors:
        mov $_sctors, %ebx
.loop_ctors:
        cmp $_ectors, %ebx
        jae .done_ctors
        call *(%ebx)
        add $4, %ebx
        jmp .loop_ctors
.done_ctors:
        ret

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
        mov $early_stack, %esp
        push %ebx
        push %eax
        call check_cpuid
        xor %eax, %eax
        mov $_sbss, %edi
        mov $_bss_size, %ecx
        rep stosb
        call run_global_ctors
        call kern_entry
        ud2
