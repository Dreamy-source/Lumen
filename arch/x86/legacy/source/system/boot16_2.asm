[org 0x8000]
[bits 16]

jmp start

SECTORS_TO_READ32    equ 32
SECTORS_TO_READ64    equ 128

%include "source/drivers/bios/print.asm"

lumen_bootloader_title: db "#####| lumen bootloader |#####", 0x0D, 0x0A, 0
lumen_default_boot_choice1: db "1. lumen default boot   | desc: default boot   | kernel", 0x0D, 0x0A, 0

boot_err_msg: db "error: unknown boot device or parameter", 0x0D, 0x0A, 0
disk_err_msg: db "error: disk error | stage 2", 0x0D, 0x0A, 0
a20_err_msg:  db "error: a20 | stage 2"

nothing_msg: db " ", 0x0D, 0x0A, 0 
lumen_boot_choice_msg: db "loading 'lumen default kernel' ...", 0x0D, 0x0A, 0

start:
    mov   si, nothing_msg
    call  print

    mov   si, lumen_bootloader_title
    call  print

    mov   si, lumen_default_boot_choice1
    call  print

    jmp   boot_choice

boot_choice:
    mov   ah, 0x00
    int   0x16
    cmp   al, '1'
    je    lumen_default_boot

    mov   si, boot_err_msg
    call  print
    jmp   boot_choice

a20_kbd_enable:
    ; A20
    ; disable keyboard
    call kbd_wait_input
    mov al, 0xAD
    out 0x64, al

    ; read P2
    call kbd_wait_input
    mov al, 0xD0
    out 0x64, al
    call kbd_wait_output
    in  al, 0x60          ; al = current P2
    
    ; bit 1 = A20 on
    or   al, 2
    mov  bl, al

    call kbd_wait_input
    mov  al, 0xD1
    out  0x64, al

    call  kbd_wait_input
    mov   al, bl
    out   0x60, al

    call kbd_wait_input
    mov al, 0xAE
    out 0x64, al
    
    ret

a20_fast_enable:
    ; A20 Fast Gate
    in  al, 0x92   ; Control Port A
    or  al, 10b    ; A20
    out 0x92, al

    ret

a20_ok:
    lgdt [gdtr]

    mov eax, cr0
    or  eax, 1b    ; PE
    mov cr0, eax

    jmp 0x08:0xC000

a20_failed:
    mov  si, a20_err_msg
    call print
    cli
    hlt

a20_test:
    push ds
    push es
    push si
    push di
    push ax
    push bx

    xor  ax, ax
    mov  es, ax
    mov  di, 0x0500

    mov ax, 0xFFFF
    mov ds, ax
    mov si, 0x0510

    mov al, [es:di]
    push ax
    mov al, [ds:si]
    push ax

    mov byte [es:di], 0xAA

    mov al, [ds:si]

    pop bx
    mov [ds:si], bl
    pop bx
    mov [es:di], bl

    cmp al, 0xAA

    pop bx
    pop ax
    pop di
    pop si
    pop es
    pop ds
    
    ret

lumen_default_boot:
    mov  si, lumen_boot_choice_msg
    call print

    mov ah, 0x42     ; BIOS Extended Read Sectors
    mov si, dap32    ; SI=DAP32
    int 0x13         ; BIOS Disk Services
    jc  disk_err

    mov ah, 0x42
    mov si, dap64
    int 0x13
    jc  disk_err

    cli
    call a20_kbd_enable
    call a20_test
    jnz  a20_ok

    call a20_fast_enable
    call a20_test
    jz   a20_failed

kbd_wait_input:
    in   al, 0x64
    test al, 2    ; input buffer full
    jnz  kbd_wait_input
    ret

kbd_wait_output:
    in   al, 0x64
    test al, 1     ; output buffer full
    jz   kbd_wait_output
    ret

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

dap32:
    db 16               ; size of packet
    db 0                ; always zero
    dw SECTORS_TO_READ32; sectors
    dw 0xC000           ; offset
    dw 0x0000           ; segment
    dq 4                ; LBA (lower 64 bits)
    dq 0                ; LBA (higher 64 bits)

dap64:
    db 16
    db 0
    dw SECTORS_TO_READ64
    dw 0x0000
    dw 0x1000
    dq 128
    dq 0