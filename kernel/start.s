.globl _start
.extern main
_start:
        mov $0x200000, %esp
        call main
        int3
