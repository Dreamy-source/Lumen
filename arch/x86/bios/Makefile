.PHONY: all build clean run-default run-hda run-hda-int run-pcspk run-pcspk-int

NASM    := nasm
CLANG   := clang
LD      := ld.lld
OBJCOPY := llvm-objcopy
DD      := dd

CFLAGS := --target=x86_64-unknown-none \
          -ffreestanding -fno-pic -fno-pie \
          -mno-red-zone -mcmodel=small \
          -Wall -Wextra -O2 -I. -Idrivers -Ilib/

LDFLAGS := --image-base 0x20000 -Ttext 0x20000

BUILD_DIR := ../../../build/x86/bios
BIN_DIR   := $(BUILD_DIR)/bin
OBJ_DIR   := $(BUILD_DIR)/obj
ELF_DIR   := $(BUILD_DIR)/elf
IMG_DIR   := $(BUILD_DIR)/img

BOOT16_1_BIN := $(BIN_DIR)/boot16-1.bin
BOOT16_2_BIN := $(BIN_DIR)/boot16-2.bin
BOOT32_BIN   := $(BIN_DIR)/boot32.bin
BOOT64_BIN   := $(BIN_DIR)/boot64.bin
BOOT64_ELF   := $(ELF_DIR)/boot64.elf
BOOT64_OBJ   := $(OBJ_DIR)/boot64.o
LUMEN_OBJ    := $(OBJ_DIR)/lumen.o
IMG          := $(IMG_DIR)/lumen.img

all: build

build: $(IMG)
	@echo "[lumenmake] build complete: $(IMG)"
	@ls -lh $(IMG)

$(BOOT16_1_BIN): system/lumenboot/boot16-1.asm
	@mkdir -p $(BIN_DIR)
	$(NASM) -f bin $< -o $@
	truncate -s 510 $@
	printf '\x55\xAA' >> $@

$(BOOT16_2_BIN): system/lumenboot/boot16-2.asm
	@mkdir -p $(BIN_DIR)
	$(NASM) -f bin $< -o $@

$(BOOT32_BIN): system/lumenboot/boot32.asm
	@mkdir -p $(BIN_DIR)
	$(NASM) -f bin $< -o $@

$(BOOT64_OBJ): system/lumenboot/boot64.asm
	@mkdir -p $(OBJ_DIR)
	$(NASM) -f elf64 $< -o $@

$(LUMEN_OBJ): system/lumen.c
	@mkdir -p $(OBJ_DIR)
	$(CLANG) $(CFLAGS) -c $< -o $@

$(BOOT64_ELF): $(BOOT64_OBJ) $(LUMEN_OBJ)
	@mkdir -p $(ELF_DIR)
	$(LD) $(LDFLAGS) -o $@ $^

$(BOOT64_BIN): $(BOOT64_ELF)
	@mkdir -p $(BIN_DIR)
	$(OBJCOPY) -O binary $< $@

$(IMG): $(BOOT16_1_BIN) $(BOOT16_2_BIN) $(BOOT32_BIN) $(BOOT64_BIN)
	@mkdir -p $(IMG_DIR)
	$(DD) if=/dev/zero of=$@ bs=512 count=2880
	$(DD) if=$(BOOT16_1_BIN) of=$@ bs=512 seek=0   conv=notrunc
	$(DD) if=$(BOOT16_2_BIN) of=$@ bs=512 seek=1   conv=notrunc
	$(DD) if=$(BOOT32_BIN)   of=$@ bs=512 seek=4   conv=notrunc
	$(DD) if=$(BOOT64_BIN)   of=$@ bs=512 seek=128 conv=notrunc

run-hda: build
	qemu-system-x86_64 \
		-drive format=raw,file=$(IMG) \
		-audiodev pa,id=snd0 \
		-device intel-hda \
		-device hda-duplex,audiodev=snd0 \
		-m 4G

run-hda-int: build
	qemu-system-x86_64 \
		-drive format=raw,file=$(IMG) \
		-audiodev pa,id=snd0 \
		-device intel-hda \
		-device hda-duplex,audiodev=snd0 \
		-m 4G -d int

run-pcspk: build
	qemu-system-x86_64 \
		-drive format=raw,file=$(IMG) \
		-audiodev pa,id=speaker \
		-machine pcspk-audiodev=speaker \
		-m 4G

run-pcspk-int: build
	qemu-system-x86_64 \
		-drive format=raw,file=$(IMG) \
		-audiodev pa,id=speaker \
		-machine pcspk-audiodev=speaker \
		-m 4G -d int

clean:
	rm -rf $(BUILD_DIR)

run-default: build
	qemu-system-x86_64 \
		-drive format=raw,file=$(IMG) \
		-audiodev pa,id=snd0 \
		-device intel-hda \
		-device hda-duplex,audiodev=snd0 \
		-m 4G \
		-machine q35 \
		-object memory-backend-ram,id=mem0,size=2G \
		-numa node,nodeid=0,memdev=mem0 \
		-object memory-backend-ram,id=mem1,size=2G \
		-numa node,nodeid=1,memdev=mem1