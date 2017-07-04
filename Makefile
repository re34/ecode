

PROJECT_NAME    :=ecode_nucleo_f303

DEFS            +=-D USE_HAL_DRIVER
DEFS            +=-D STM32F303xE

CROSS_COMPILE   :=arm-none-eabi-

CC      :=$(CROSS_COMPILE)gcc
AS      :=$(CROSS_COMPILE)as
LD      :=$(CROSS_COMPILE)ld
AR      :=$(CROSS_COMPILE)ar
OBJCP   :=$(CROSS_COMPILE)objcopy
OBJSIZE :=$(CROSS_COMPILE)size


CCFLAGS		+=-mcpu=cortex-m4 -mthumb -Wall -Os -std=gnu99
CCFLAGS		+=-mfpu=fpv4-sp-d16 -mfloat-abi=softfp
CCFLAGS		+=-Wno-unused-variable	#don't warning unused variable
CCFLAGS		+=-nostartfiles
CCFLAGS		+=-ffunction-sections -fdata-sections
CCFLAGS		+=-g

LDFLAGS		+=-mcpu=cortex-m4 -mthumb
#LDFLAGS		+=-specs=nano.specs
LDFLAGS		+=-u_printf_float
LDFLAGS		+=-Wl,--gc-sections
LDFLAGS		+=-mfpu=fpv4-sp-d16 -mfloat-abi=softfp

LINK_FILE   = $(PROJECT_DIR)/user/nucleo_f303/gcc_toolchain/STM32XE_FLASH.ld

PROJECT_DIR     :=.
BUILD_DIR       :=$(PROJECT_DIR)/build
OUTPUT_DIR      :=$(PROJECT_DIR)/OBJ

TARGET_ELF      :=$(OUTPUT_DIR)/$(PROJECT_NAME).elf
TARGET_BIN      :=$(OUTPUT_DIR)/$(PROJECT_NAME).bin
TARGET_HEX      :=$(OUTPUT_DIR)/$(PROJECT_NAME).hex

SOURCE_DIR      :=$(PROJECT_DIR)/common
SOURCE_DIR      +=$(PROJECT_DIR)/drivers/chip
#SOURCE_DIR      +=$(PROJECT_DIR)/eth
SOURCE_DIR      +=$(PROJECT_DIR)/drivers/iic
SOURCE_DIR      +=$(PROJECT_DIR)/drivers/oled
SOURCE_DIR      +=$(PROJECT_DIR)/drivers/pin
SOURCE_DIR      +=$(PROJECT_DIR)/drivers/serial
SOURCE_DIR      +=$(PROJECT_DIR)/drivers/spi
SOURCE_DIR      +=$(PROJECT_DIR)/drivers/timer
SOURCE_DIR      +=$(PROJECT_DIR)/module/cli
SOURCE_DIR      +=$(PROJECT_DIR)/platform
SOURCE_DIR      +=$(PROJECT_DIR)/user/nucleo_f303/applications
SOURCE_DIR      +=$(PROJECT_DIR)/user/nucleo_f303/drivers/
SOURCE_DIR      +=$(PROJECT_DIR)/user/nucleo_f303/CMSIS/
SOURCE_DIR      +=$(PROJECT_DIR)/user/nucleo_f303/STMF3xx_HAL_Driver/Src
SOURCE_DIR      +=$(PROJECT_DIR)/utils
SOURCE_DIR      +=$(PROJECT_DIR)/library/FreeRTOS
SOURCE_DIR      +=$(PROJECT_DIR)/portable/MemMang
SOURCE_DIR      +=$(PROJECT_DIR)/portable/GCC/ARM_CM3

SOURCE_ASM      :=$(PROJECT_DIR)/user/nucleo_f303/gcc_toolchain

INCLUDE_DIR     :=
INCLUDE_DIR     +=$(SOURCE_DIR)
INCLUDE_DIR     +=$(PROJECT_DIR)/drivers
INCLUDE_DIR     +=$(PROJECT_DIR)/library/FreeRTOS/include
INCLUDE_DIR     +=$(PROJECT_DIR)/module

includes        :=$(addprefix -I,$(INCLUDE_DIR))

#$(call source_to_obj,source_to_obj)
source_to_obj=$(subst .c,.o,$1)

c_sources       :=$(shell find $(SOURCE_DIR) -name "*.c")

c_objs          :=$(addprefix $(BUILD_DIR)/,$(call source_to_obj,$(sources)))

asm_sources     :=$(shell find $(SOURCE_ASM) -name "*.s")
asm_objs        :=$(addprefix $(BUILD_DIR)/,$(subst .s,.o,$(asm_sources)))
obj_dirs      :=$(patsubst %.o,%,$(c_objs))
obj_dirs      +=$(patsubst %.s,%,$(asm_objs))



create_obj_dirs :=$(shell for f in $(obj_dirs);\
                        do                      \
                            test -d $$f||mkdir -p $$f;\
                        done)
                        
$(TARGET_BIN):$(TARGET_ELF)
    $(OBJCP) -O binary -S $< $@
    
$(TARGET_ELF):$(c_objs) $(asm_objs)
    $(CC) $(LDFLAGS) $^ -T $(LINK_FILE) -o $@
    $(OBJSIZE) $@
    
$(c_objs):$(BUILD_DIR)/%.o:%.c
    $(CC) $(CCFLAGS) $(DEFS) $(includes) -c $< -o $@
    
$(asm_objs):$(BUILD_DIR)/%.o:%.s
    $(CC) $(CCFLAGS) -c $< -o $@
    
clean:
    rm -rf $(c_objs)
    rm -rf $(asm_objs)
    rm -rf $(TARGET_BIN)
    rm -rf $(TARGET_ELF)
    rm -rf $(TARGET_HEX)
                        




