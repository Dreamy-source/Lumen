[org 0x7c00]
[bits 16]

jmp start

%include "source/drivers/bios/descblock.asm"
%include "source/drivers/bios/print.asm"

boot_drive: db 0x00

disk_err_msg: db "[error]: disk error", 0x0D, 0x0A, 0

start:
    cli
    mov [boot_drive], dl

    mov ah, 0x42   ; BIOS Extended Read Sectors
    mov dl, [boot_drive]
    mov si, dap    ; SI=DAP
    int 0x13       ; BIOS Disk Services
    jc  disk_err

    in  al, 0x92   ; Control Port A
    or  al, 10b    ; A20
    out 0x92, al

    lgdt [gdtr]

    mov eax, cr0
    or  eax, 1b    ; PE
    mov cr0, eax

    jmp 0x08:KERNEL_ADDRESS

gdt:
    dq 0   ; null descriptor

    ; code segment (ring 0)
    dw 0xFFFF       ; limit 15:0
    dw 0x000        ; base 15:0
    db 0x00         ; base 23:16
    db 10011010b    ; access: P=1, DPL=0, S=1, E=1, RW=1
    db 11001111b    ; flags: G=1, D=1, Limit 19:16=0xF
    db 0x00         ; base 31:24

    ; data segment (ring 0)
    dw 0xFFFF       ; limit 15:0
    dw 0x0000       ; base 15:0
    db 0x00         ; base 23:16
    db 10010010b    ; access: P=1, DPL=0, S=1, E=0, RW=1
    db 11001111b    ; flags: G=1, D=1, Limit 19:16=0xF
    db 0x00         ; base 31:24

    ; short version:
    ; dq 0x0000000000000000   ; null descriptor
    ; dq 0x00CF9A000000FFFF   ; code segment (ring 0)
    ; dq 0x00CF92000000FFFF   ; data segment (ring 0)

gdt_end:

gdtr:
    dw gdt_end - gdt - 1
    dd gdt

disk_err:
    mov  si, disk_err_msg
    call print
    cli
    hlt

dap:
    db 16               ; size of packet
    db 0                ; always zero
    dw SECTORS_TO_READ  ; sectors
    dw KERNEL_ADDRESS   ; offset
    dw 0x0000           ; segment
    dq 1                ; LBA (lower 64 bits)
    dq 0                ; LBA (higher 64 bits)