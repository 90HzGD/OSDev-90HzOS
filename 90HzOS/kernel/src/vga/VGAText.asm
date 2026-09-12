[BITS 32]

global update_cursor 
extern outb

pos_low: db 0
pos_high: db 0

update_cursor: 
    mov eax, [esp + 4] 
    mov ecx, [esp + 8]
    mov [pos_low], cl
    mov [pos_high], al
    mov eax, 0
    mov ecx, 0
    
    push dword 0x0E 
    push dword 0x3D4 
    call outb 
    add esp, 8 
    
    mov ecx, [pos_low]
    push ecx 
    push dword 0x3D5 
    call outb 
    add esp, 8 
    
    push dword 0x0F 
    push dword 0x3D4 
    call outb 
    add esp, 8

    mov eax, [pos_high]
    push eax 
    push dword 0x3D5 
    call outb 
    add esp, 8 
    ret