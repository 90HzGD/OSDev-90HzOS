[BITS 16]
[org 0x7c00]

_start:
    jmp short start
    nop

BOOT_DISK: db 0

%define STACK_ADR 0x3000

start:
    cli
    ; Setup stack
    xor ax, ax
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov gs, ax
    mov bp, STACK_ADR
    mov sp, bp
    sti

    mov si, Begin_string
    call Print_string16

    mov [BOOT_DISK], dl
    xor ax, ax
    int 0x13
    jc halt

    mov ah, 0x42
    mov dl, [BOOT_DISK]
    mov si, DAP_stage2
    int 0x13

    mov si, Disk_Error
    jc halt

    mov si, Pass_string
    call Print_string16
    mov dl, [BOOT_DISK]
    jmp 0x0000:0x5500       ; Long jump to stage 2

halt:
    call Print_string16
    hang:
        hlt
        jmp hang

DAP_stage2:
    .size:      db 0x10
    .reserved:  db 0x00
    .sectors:   dw 0x02
    .offset:    dw 0x5500
    .segment:   dw 0x0000
    .lba:       dq 2048

Print_string16:
    cmp [si], 0
    je return_str16
    jmp Print_char16
Print_char16:
    mov ah, 0x0E
    mov al, [si]
    int 0x10
    add si, 1
    jmp Print_string16
return_str16:
    mov si, 0
    ret

Disk_Error: db "Disk Error.", 0
Begin_string: db "In bootloader...", 0
Pass_string: db "Loaded stage2", 0

times 446-($-$$) db 0

Partition_MBR_Begin:
    db 0x80                 ; bootable partition
    db 0x00, 0x02, 0x00     ; CHS begin (sus)
    db 0x7F                 ; Custom OS
    db 0xFF, 0xFF, 0xFF     ; CHS End (sus again)
    dd 2048                 ; Partition #1 LBA
    dd 1000000              ; ~488 Mio
    times (16 * 3) db 0
Partition_MBR_End:

dw 0xAA55