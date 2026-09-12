[BITS 32]

global update_cursor
extern outb

update_cursor:
    push ebx

    mov ebx, [esp + 8] 

    push dword 0x0E
    push dword 0x3D4
    call outb
    add esp, 8

    mov eax, ebx
    shr eax, 8
    and eax, 0xFF 
    push eax
    push dword 0x3D5
    call outb
    add esp, 8

    push dword 0x0F
    push dword 0x3D4
    call outb
    add esp, 8

    mov eax, ebx
    and eax, 0xFF
    push eax
    push dword 0x3D5
    call outb
    add esp, 8
    
    pop ebx
    ret
