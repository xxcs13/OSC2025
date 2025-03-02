SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

CFLAGS = -Iinclude -fno-stack-protector -Wall -Wextra -Wpedantic -O2 \
         -ffreestanding -nostdinc -nostdlib -nostartfiles \
         -ffunction-sections -fdata-sections \
         -I../../../opt/lib/gcc/aarch64-linux-gnu/13.3.0/include
		 

LDFLAGS = --gc-sections -T linker.ld

all: kernel8.img

start.o: start.S
    aarch64-linux-gnu-gcc $(CFLAGS) -c start.S -o start.o

%.o: %.c
    aarch64-linux-gnu-gcc $(CFLAGS) -c $< -o $@

kernel8.img: start.o $(OBJS)
    aarch64-linux-gnu-ld $(LDFLAGS) start.o $(OBJS) -o kernel8.elf
    aarch64-linux-gnu-objcopy -O binary kernel8.elf kernel8.img

clean:
    rm -f kernel8.elf kernel8.img *.o

run:
    qemu-system-aarch64 -M raspi3b -serial null -serial stdio \
        -initrd initramfs.cpio -dtb bcm2710-rpi-3-b-plus.dtb -display none \
        -kernel kernel8.img
