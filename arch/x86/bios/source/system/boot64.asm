[bits 64]

extern lumen_main

global _start

extern irq0_handler_c
extern irq1_handler_c

global irq0_handler_asm
global irq1_handler_asm

_start:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov rsp, 0x200000

    mov rdi,        0xB8000
    mov [rdi],      'm'
    mov [rdi + 80], 0x07 
    mov [rdi + 81], '6'
    mov [rdi + 82], 0x07 
    mov [rdi + 83], '4'
    mov [rdi + 84], 0x07 
    
    call lumen_main

    jmp $

irq0_handler_asm:
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    call irq0_handler_c

    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rbp
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax

    iretq

irq1_handler_asm:
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    call irq1_handler_c

    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rbp
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax

    iretq