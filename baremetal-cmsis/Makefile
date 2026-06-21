TARGET := baremetal-cmsis

# Define directories
BUILD_DIR := build

# Toolchain
CC := arm-none-eabi-gcc
AS := arm-none-eabi-gcc
OBJCOPY := arm-none-eabi-objcopy
SIZE := arm-none-eabi-size


# MCU and include Flags
MCU_FLAGS := -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard
ASFLAGS := $(MCU_FLAGS)
DEFS := -DSTM32F303x8
INCLUDES := -Iinclude -Ilib/cmsis_core -Ilib/cmsis_device

# Compiler flags
CFLAGS := $(MCU_FLAGS) $(DEFS) $(INCLUDES) -std=c11 -Wall -Wextra -ffunction-sections -fdata-sections -MMD -MP

# Map source to build objects
C_SOURCES := src/main.c src/system_stm32f3xx.c src/clocks.c src/led.c src/spi.c src/systick.c src/uart.c
ASM_SOURCES := startup/startup_stm32f303x8.s
C_OBJS := $(C_SOURCES:%.c=$(BUILD_DIR)/%.o)
ASM_OBJS := $(ASM_SOURCES:%.s=$(BUILD_DIR)/%.o)
OBJS := $(C_OBJS) $(ASM_OBJS)

# Linker flags
LDSCRIPT := linker/STM32F303K8Tx_FLASH.ld
LDFLAGS := $(MCU_FLAGS) -T$(LDSCRIPT)
LDFLAGS += -Wl,--gc-sections -Wl,-Map=$(BUILD_DIR)/$(TARGET).map
LDFLAGS += --specs=nano.specs --specs=nosys.specs

all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin

$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean

-include $(OBJS:.o=.d)
