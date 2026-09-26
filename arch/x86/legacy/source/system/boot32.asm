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

    ; cr3 = pml4
    mov eax, pml4
    mov cr3, eax

    ; pae
    mov eax, cr4
    or  eax, 1 << 5
    mov cr4, eax
    
    ; efer
    mov ecx, 0xC0000080
    rdmsr
    or  eax, 1 << 8
    wrmsr

    ; paging
    mov eax, cr0
    or  eax, 1 << 31
    mov cr0, eax

    lgdt [gdtr]

    jmp 0x08:0x10000

gdt:
    dq 0                    ; null descriptor
    dq 0x00209A0000000000   ; code descriptor
    dq 0x0000920000000000   ; data descriptor

gdtr:
    dw $ - gdt - 1
    dd gdt

align 4096
pml4:
    dq pdpt + 0x03  ; present + writable
    times 512-1 dq 0

align 4096
pdpt:
    dq pd + 0x03
    times 512-1 dq 0

align 4096
pd:
    dq 0x83             ; 0x000000–0x1FFFFF
    dq 0x83 + 0x200000  ; 0x200000–0x3FFFFF
    times 510 dq 0