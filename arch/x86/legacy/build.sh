clear

nasm -f bin source/system/boot16.asm -o build/bin/boot16.bin
nasm -f bin source/system/boot32.asm -o build/bin/boot32.bin
nasm -f elf64 source/system/boot64.asm -o build/obj/boot64.o

clang --target=x86_64-unknown-none -c source/system/lumen.c -o build/obj/lumen.o \
    -ffreestanding -fno-pic -fno-pie -mno-red-zone \
    -mcmodel=small -Wall -Wextra -O2

ld.lld --image-base 0x10000 -Ttext 0x10000 -o build/elf/boot64.elf \
    build/obj/boot64.o build/obj/lumen.o

llvm-objcopy -O binary build/elf/boot64.elf build/bin/boot64.bin

truncate -s 510 build/bin/boot16.bin
printf '\x55\xAA' >> build/bin/boot16.bin

dd if=/dev/zero of=build/img/lumen.img bs=512 count=2880
dd if=build/bin/boot16.bin of=build/img/lumen.img bs=512 seek=0   conv=notrunc
dd if=build/bin/boot32.bin of=build/img/lumen.img bs=512 seek=1   conv=notrunc
dd if=build/bin/boot64.bin of=build/img/lumen.img bs=512 seek=128 conv=notrunc

qemu-system-x86_64 \
    -drive format=raw,file=build/img/lumen.img