[bits 64]

extern lumen_main

global _start

_start:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov rsp, 0x200000
    
    call lumen_main

    jmp $