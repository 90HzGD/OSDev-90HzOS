[BITS 32]

global _start
global freeze
global Get_CPU_name
global update_stackptr
extern main
extern clear_screen

_start:
    mov eax, 0
    mov ecx, 0

    call main

    jmp shutdown

freeze:
    jmp $

Get_CPU_name:
    pushad
    mov eax, 0x80000002
    mov esi, CPUName_str
    ._loop:
        cmp eax, 0x80000004
        ja Get_CPU_name.end
        push eax
        cpuid
        mov [esi], eax
        add esi, 4
        mov [esi], ebx
        add esi, 4
        mov [esi], ecx
        add esi, 4
        mov [esi], edx
        pop eax
        inc eax
        jmp Get_CPU_name._loop
    .end:
    popad
    mov eax, CPUName_str
    ret

CPUName_str: dq 0,0,0,0,0,0

update_stackptr:
    pushad
    mov eax, [esp + 4]
    mov ebp, eax
    mov esp, eax
    ret

shutdown:
    call clear_screen
    mov esi, end_string
    mov ecx, 0
    call print_string
    sti
    hang:
        hlt         ; Set CPU as idle
        jmp hang    ; infinite loop for no triple fault

print_string:
    cmp byte [esi], 0
    je return
    jne print_char
print_char:
    mov al, [esi]
    mov [0xB8000+ecx], al
    mov [0xB8001+ecx], 0x04
    add ecx, 2
    add esi, 1
    jmp print_string
return:
    mov esi, 0
    mov ecx, 0
    mov eax, 0
    ret

end_string: db "Its now safe to turn off your computer", 0