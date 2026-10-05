clear

gcc system/boot.c                      \
        -c                             \
        -fno-stack-protector           \
        -fpic                          \
        -fshort-wchar                  \
        -mno-red-zone                  \
        -I/usr/include/efi             \
        -I/usr/include/efi/x86_64      \
        -I/usr/include/efi/protocol    \
        -DEFI_FUNCTION_WRAPPER         \
        -o build/obj/boot.o

ld build/obj/boot.o                    \
        /usr/lib/crt0-efi-x86_64.o     \
        -nostdlib                      \
        -znocombreloc                  \
        -T /usr/lib/elf_x86_64_efi.lds \
        -shared                        \
        -Bsymbolic                     \
        -L /usr/lib                    \
        -l:libgnuefi.a                 \
        -l:libefi.a                    \
        -o build/so/boot.so

objcopy -j .text                       \
        -j .sdata                      \
        -j .data                       \
        -j .rodata                     \
        -j .dynamic                    \
        -j .dynsym                     \
        -j .rel                        \
        -j .rela                       \
        -j .reloc                      \
        --output-target=efi-app-x86_64 \
        build/so/boot.so               \
        build/efi/boot.efi


cp build/efi/boot.efi lumenboot/EFI/BOOT/BOOTX64.EFI

qemu-system-x86_64                                                                   \
    -drive if=pflash,format=raw,readonly=on,file=/home/dreamy/OVMF_CODE.fd           \
    -drive format=raw,file=fat:rw:lumenboot                                          \
    -machine q35                                                                     \
    -serial stdio                                                                    \
    -m 512                                                                           