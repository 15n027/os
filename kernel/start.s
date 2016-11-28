[bits 32]

global _start
extern main
_start:
        mov esp, 0x200000
        call main
