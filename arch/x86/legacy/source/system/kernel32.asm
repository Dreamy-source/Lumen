[org 0x8000]
[bits 32]

start:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000

    mov edi, 0xB8000
    mov dword [edi], "32"
    mov [edi + 2], 0x0A
    
    jmp $