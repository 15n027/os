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

_start:
        mov $early_stack, %esp
        push %ebx
        push %eax
        push $0
        popf
        xor %eax, %eax
        mov $_sbss, %edi
        mov $_bss_size, %ecx
        rep stosb
        call run_global_ctors
        call kern_entry
        int3
