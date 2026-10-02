[org 0x7c00]
[bits 16]

jmp start

SECTORS_TO_READ16    equ 32

%include "source/drivers/bios/print.asm"

disk_err_msg: db "error: disk error | stage 1", 0x0D, 0x0A, 0

boot_drive: db 0x00

; lumen memory map:
; boot 16:     0x7C00
; boot 32:     0x8000
; boot 64:     0x10000

start:
    mov   [boot_drive], dl
    mov   sp, 0x7c00

    jmp   lumen_boot2

lumen_boot2:
    mov ah, 0x42
    mov dl, [boot_drive]
    mov si, dap16
    int 0x13
    jc  disk_err

    jmp 0x0000:0x8000

dap16:
    db 16               ; size of packet
    db 0                ; always zero
    dw SECTORS_TO_READ16; sectors
    dw 0x8000           ; offset
    dw 0x0000           ; segment
    dq 1                ; LBA (lower 64 bits)
    dq 0                ; LBA (higher 64 bits)

disk_err:
    mov  si, disk_err_msg
    call print
    cli
    hlt