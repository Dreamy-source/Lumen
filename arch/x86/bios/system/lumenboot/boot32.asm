[org 0xC000]
[bits 32]

TABLE_SIZE                equ 0x200000  ; 2 MB
PAGE_SIZE                 equ 4096      ; 4 KB
PRESENT_WRITABLE_PAGESIZE equ 10000011b ; Present + Writable + PageSize
; bit 0 - present
; bit 1 - writable
; bit 7 - page size (2 MB)

PRESENT_WRITABLE          equ 00000011b ; Present + Writable
; bit 0 - present
; bit 1 - writable

start:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000

    mov edi, 0xB8000
    mov ax,  0x2000
    mov cx,  80 * 25
    rep stosb

    mov edi,       0xB8000
    mov [edi],     'm'
    mov [edi + 1], 0x07 
    mov [edi + 2], '3'
    mov [edi + 3], 0x07 
    mov [edi + 4], '2'
    mov [edi + 5], 0x07 

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

    jmp 0x08:0x20000

gdt:
    dq 0                    ; null descriptor
    dq 0x00209A0000000000   ; code descriptor
    dq 0x0000920000000000   ; data descriptor

gdtr:
    dw $ - gdt - 1
    dq gdt

align PAGE_SIZE
pml4:
    dq pdpt + PRESENT_WRITABLE
    times 512-1 dq 0

align PAGE_SIZE
pdpt:
    dq pd0 + PRESENT_WRITABLE
    dq pd1 + PRESENT_WRITABLE
    dq pd2 + PRESENT_WRITABLE
    dq pd3 + PRESENT_WRITABLE
    times 512-4 dq 0

align PAGE_SIZE
pd0:
    %assign i 0 * 0
    %rep 512
        dq (i * TABLE_SIZE) + PRESENT_WRITABLE_PAGESIZE
        %assign i i + 1
    %endrep

align PAGE_SIZE
pd1:
    %assign i 512 * 1
    %rep 512
        dq (i * TABLE_SIZE) + PRESENT_WRITABLE_PAGESIZE
        %assign i i + 1
    %endrep

align PAGE_SIZE
pd2:
    %assign i 512 * 2
    %rep 512
        dq (i * TABLE_SIZE) + PRESENT_WRITABLE_PAGESIZE
        %assign i i + 1
    %endrep

align PAGE_SIZE
pd3:
    %assign i 512 * 3
    %rep 512
        dq (i * TABLE_SIZE) + PRESENT_WRITABLE_PAGESIZE
        %assign i i + 1
    %endrep