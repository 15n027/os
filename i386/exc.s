bits 32
section .text
global exc_de
global exc_db
global exc_nmi
global exc_bp
global exc_of
global exc_br
global exc_ud
global exc_nm
global exc_df
global exc_ts
global exc_np
global exc_ss
global exc_gp
global exc_pf
global exc_mf
global exc_ac
global exc_mc
global exc_xf


extern exception_handlers

exc_de:
        push 0
        push 0
        jmp exc_default
exc_db:
        push 0
        push 1
        jmp exc_default
exc_nmi:
        push 0
        push 2
        jmp exc_default
exc_bp:
        push 0
        push 3
        jmp exc_default
exc_of:
        push 0
        push 4
        jmp exc_default
exc_br:
        push 0
        push 5
        jmp exc_default
exc_ud:
        push 0
        push 6
        jmp exc_default
exc_nm:
        push 0
        push 7
        jmp exc_default
exc_df:
        push 8
        jmp exc_default
exc_ts:
        push 10
        jmp exc_default
exc_np:
        push 11
        jmp exc_default
exc_ss:
        push 12
        jmp exc_default
exc_gp:
        push 13
        jmp exc_default
exc_pf:
        push 14
        jmp exc_default
exc_mf:
        push 0
        push 16
        jmp exc_default
exc_ac:
        push 17
        jmp exc_default
exc_mc:
        push 0
        push 18
        jmp exc_default
exc_xf:
        push 0
        push 19
        jmp exc_default

exc_default:
        push eax
        push ebx
        push ecx
        push edx
        push edi
        push esi
        push ebp
        mov eax, [esp + 7 * 4]
        call [exception_handlers + eax * 4]

        pop ebp
        pop esi
        pop edi
        pop edx
        pop ecx
        pop ebx
        pop eax
        add esp, 8
clihlt:
        cli
        hlt
        jmp clihlt
        iret
