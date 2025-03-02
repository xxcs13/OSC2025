CROSS_COMPILE = aarch64-linux-gnu

CFLAGS = -Wall -nostdlib -nostartfiles -ffreestanding -Iinclude -mgeneral-regs-only
# -nostdlib Don't use the C standard library. 
# -nostartfiles Don't use standard startup files. Startup files are responsible for setting an initial stack pointer, initializing static data, and jumping to the main entry point. 
# -ffreestanding A freestanding environment is an environment in which the standard library may not exist, and program startup may not necessarily be at main. The option -ffreestanding directs the compiler to not assume that standard functions have their usual definition.
# -mgeneral-regs-only. Use only general-purpose registers. ARM processors also have NEON registers. We don't want the compiler to use them because they add additional complexity 
# -nostdinc Do not search for system include library
# -Iinclude Search for header files in the include folder.
ASMFLAGS = -Iinclude 

BUILD_DIR = build
SRC_DIR = src

all : kernel8.img

clean :
	rm -rf $(BUILD_DIR) *.img 

$(BUILD_DIR)/%_c.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(CROSS_COMPILE)-gcc $(CFLAGS) -MMD -c $< -o $@

$(BUILD_DIR)/%_s.o: $(SRC_DIR)/%.S
	$(CROSS_COMPILE)-gcc $(ASMFLAGS) -MMD -c $< -o $@

C_FILES = $(wildcard $(SRC_DIR)/*.c)
ASM_FILES = $(wildcard $(SRC_DIR)/*.S)
OBJ_FILES = $(C_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%_c.o)
OBJ_FILES += $(ASM_FILES:$(SRC_DIR)/%.S=$(BUILD_DIR)/%_s.o)
DEP_FILES = $(OBJ_FILES:%.o=%.d)
-include $(DEP_FILES)

# Recursively compile .S or .c files to .o
kernel8.img: $(SRC_DIR)/linker.ld $(OBJ_FILES)
	$(CROSS_COMPILE)-ld -T $(SRC_DIR)/linker.ld -o $(BUILD_DIR)/kernel8.elf  $(OBJ_FILES)
	$(CROSS_COMPILE)-objcopy $(BUILD_DIR)/kernel8.elf -O binary kernel8.img
