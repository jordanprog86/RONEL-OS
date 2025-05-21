# Compiler and assembler settings
ASM = nasm
CC = g++
LD = ld

# Flags
ASMFLAGS = -f bin
CFLAGS = -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -c
LDFLAGS = -m elf_i386 -T linker.ld -nostdlib

# Files
BOOTLOADER = boot.bin
KERNEL = kernel.bin
OS_IMAGE = ronel_os.img
OBJECTS = kernel.o calculator.o screen.o

# Build targets
all: $(OS_IMAGE)

$(BOOTLOADER): boot.asm
	$(ASM) $(ASMFLAGS) boot.asm -o $(BOOTLOADER)

kernel.o: kernel.cpp screen.h
	$(CC) $(CFLAGS) kernel.cpp -o kernel.o

calculator.o: calculator.cpp screen.h
	$(CC) $(CFLAGS) calculator.cpp -o calculator.o

screen.o: screen.cpp screen.h
	$(CC) $(CFLAGS) screen.cpp -o screen.o

$(KERNEL): $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $(KERNEL)

$(OS_IMAGE): $(BOOTLOADER) $(KERNEL)
	dd if=/dev/zero of=$(OS_IMAGE) bs=512 count=2880
	dd if=$(BOOTLOADER) of=$(OS_IMAGE) conv=notrunc
	dd if=$(KERNEL) of=$(OS_IMAGE) bs=512 seek=1 conv=notrunc

# Clean build files
clean:
	rm -f *.o *.bin *.img

# Run in QEMU
run: $(OS_IMAGE)
	qemu-system-i386 -drive format=raw,file=$(OS_IMAGE)

.PHONY: all clean run 
