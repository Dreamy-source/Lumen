clear

nasm -f bin source/system/boot.asm -o build/bin/boot.bin
nasm -f bin source/system/kernel32.asm -o build/bin/kernel32.bin

truncate -s 510 build/bin/boot.bin
printf '\x55\xAA' >> build/bin/boot.bin

cat build/bin/boot.bin build/bin/kernel32.bin > build/img/lumen.img
qemu-system-x86_64 \
    -drive format=raw,file=build/img/lumen.img