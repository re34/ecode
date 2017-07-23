
PROJECT         =ecode
PROJECT_DIR		=.

BOARD_NAME     =nucleo_f767
LDSCRIPT_FILE   =STM32F767ZI_FLASH.ld
STARTUP_FILE    =startup_stm32f767xx.s
CHIP_TYPE       =STM32F7xx

DEFS        +=-D USE_HAL_DRIVER
DEFS        +=-D STM32F767xx

CCFLAGS		+=-mcpu=cortex-m7 -mthumb -Wall -Os -std=gnu99
CCFLAGS		+=-mfpu=fpv5-sp-d16 -mfloat-abi=softfp
CCFLAGS		+=-Wno-unused-variable	#don't warning unused variable
CCFLAGS		+=-nostartfiles
CCFLAGS		+=-ffunction-sections -fdata-sections
CCFLAGS		+=-g

LDFLAGS		+=-mcpu=cortex-m7 -mthumb
LDFLAGS		+=-specs=nano.specs
LDFLAGS		+=-u_printf_float
LDFLAGS		+=-Wl,--gc-sections
LDFLAGS		+=-mfpu=fpv5-sp-d16 -mfloat-abi=softfp

INC_DIR		+=-I $(PROJECT_DIR)/common
INC_DIR		+=-I $(PROJECT_DIR)/drivers
INC_DIR		+=-I $(PROJECT_DIR)/library/FreeRTOS/include
INC_DIR		+=-I $(PROJECT_DIR)/library/FreeRTOS/portable/GCC/ARM_CM7/r0p1
INC_DIR		+=-I $(PROJECT_DIR)/library/lwip/include
#INC_DIR		+=-I $(PROJECT_DIR)/library/lwip/include/netif
INC_DIR		+=-I $(PROJECT_DIR)/library/lwip/include/ipv4
INC_DIR		+=-I $(PROJECT_DIR)/library/lwip/arch
INC_DIR		+=-I $(PROJECT_DIR)/library/lwip
INC_DIR		+=-I $(PROJECT_DIR)/module
INC_DIR		+=-I $(PROJECT_DIR)/platform
INC_DIR		+=-I $(PROJECT_DIR)/user/$(BOARD_NAME)/applications
INC_DIR		+=-I $(PROJECT_DIR)/user/$(BOARD_NAME)/CMSIS
INC_DIR		+=-I $(PROJECT_DIR)/user/$(BOARD_NAME)/drivers
INC_DIR		+=-I $(PROJECT_DIR)/user/$(BOARD_NAME)/$(CHIP_TYPE)_HAL_Driver/Inc
INC_DIR		+=-I $(PROJECT_DIR)/utils
INC_DIR     +=-I $(PROJECT_DIR)/library/lua

c_sources   +=$(wildcard $(PROJECT_DIR)/library/FreeRTOS/*.c)
c_sources   +=$(wildcard $(PROJECT_DIR)/library/FreeRTOS/portable/GCC/ARM_CM7/r0p1/*.c)
c_sources   +=$(PROJECT_DIR)/library/FreeRTOS/portable/MemMang/heap_4.c
c_sources   +=$(wildcard $(PROJECT_DIR)/library/lwip/api/*.c)
c_sources   +=$(wildcard $(PROJECT_DIR)/library/lwip/arch/*.c)
c_sources   +=$(wildcard $(PROJECT_DIR)/library/lwip/core/*.c)
c_sources   +=$(wildcard $(PROJECT_DIR)/library/lwip/core/ipv4/*.c)
c_sources   +=$(wildcard $(PROJECT_DIR)/library/lwip/netif/*.c)
c_sources   +=$(wildcard $(PROJECT_DIR)/drivers/chip/*.c)
c_sources   +=$(wildcard $(PROJECT_DIR)/drivers/iic/*.c)
c_sources   +=$(wildcard $(PROJECT_DIR)/drivers/oled/*.c)
c_sources   +=$(wildcard $(PROJECT_DIR)/drivers/pin/*.c)
c_sources   +=$(wildcard $(PROJECT_DIR)/drivers/serial/*.c)
c_sources   +=$(wildcard $(PROJECT_DIR)/drivers/spi/*.c)
c_sources   +=$(wildcard $(PROJECT_DIR)/drivers/timer/*.c)
c_sources   +=$(wildcard $(PROJECT_DIR)/drivers/eth/*.c)
c_sources   +=$(wildcard $(PROJECT_DIR)/module/cli/*.c)
c_sources   +=$(wildcard $(PROJECT_DIR)/platform/*.c)
c_sources   +=$(wildcard $(PROJECT_DIR)/user/$(BOARD_NAME)/applications/*.c)
c_sources   +=$(wildcard $(PROJECT_DIR)/user/$(BOARD_NAME)/drivers/*.c)
c_sources   +=$(wildcard $(PROJECT_DIR)/user/$(BOARD_NAME)/$(CHIP_TYPE)_HAL_Driver/Src/*.c)
c_sources   +=$(wildcard $(PROJECT_DIR)/utils/*.c)
c_sources   +=$(wildcard $(PROJECT_DIR)/library/lua/*.c)
c_sources   +=$(wildcard $(PROJECT_DIR)/module/lua/*.c)

LDSCRIPT        =$(PROJECT_DIR)/user/$(BOARD_NAME)/gcc_toolchain/$(LDSCRIPT_FILE)

CROSS_COMPILE   :=arm-none-eabi-


BUILD_DIR         =$(PROJECT_DIR)/build
BIN_DIR      =$(PROJECT_DIR)/bin

TARGET_ELF	=$(BUILD_DIR)/$(PROJECT)_$(BOARD_NAME).elf
TARGET_BIN	=$(BUILD_DIR)/$(PROJECT)_$(BOARD_NAME).bin
TARGET_HEX	=$(BUILD_DIR)/$(PROJECT)_$(BOARD_NAME).hex


OBJCPFLAGS_ELF_TO_BIN	=-Obinary
OBJCPFLAGS_ELF_TO_HEX	=-Oihex
OBJCPFLAGS_BIN_TO_HEX	=-Ibinary -Oihex


CC		:=$(CROSS_COMPILE)gcc
AS		:=$(CROSS_COMPILE)as
LD		:=$(CROSS_COMPILE)ld
AR		:=$(CROSS_COMPILE)ar
OBJCP		:=$(CROSS_COMPILE)objcopy
OBJSIZE		:=$(CROSS_COMPILE)size


asm_sources =$(PROJECT_DIR)/user/$(BOARD_NAME)/gcc_toolchain/$(STARTUP_FILE)


#$(call c_sources_to_objs,c_source_list)
c_sources_to_objs=$(patsubst $(PROJECT_DIR)/%,$(BUILD_DIR)/%,$(subst .c,.o,$1))

asm_sources_to_objs=$(patsubst $(PROJECT_DIR)/%,$(BUILD_DIR)/%,$(subst .s,.o,$1))

c_objs  :=$(call c_sources_to_objs,$(c_sources))
asm_objs:=$(call asm_sources_to_objs,$(asm_sources))

objs_dir :=$(dir $(c_objs))
objs_dir +=$(dir $(asm_objs))

create_obj_dir :=$(shell for f in $(objs_dir);\
			do			\
				test -d $$f || mkdir -p $$f;\
			done)



.PHONY: all clean


all: $(TARGET_BIN) $(TARGET_ELF)


$(TARGET_BIN):$(TARGET_ELF)
	$(OBJCP) -O binary -S $< $@

$(TARGET_ELF):$(asm_objs) $(c_objs)
	$(CC) $(LDFLAGS) $^ -T $(LDSCRIPT) -o $@
	$(OBJSIZE) $@


$(c_objs):$(BUILD_DIR)/%.o:$(PROJECT_DIR)/%.c
	$(CC) $(CCFLAGS) $(DEFS) $(INC_DIR) -c $< -o  $@

$(asm_objs):$(BUILD_DIR)/%.o:$(PROJECT_DIR)/%.s
	$(CC) $(CCFLAGS) -c $< -o $@


clean:
	$(RM) $(c_objs) 
	$(RM) $(asm_objs) 
	$(RM) $(TARGET_BIN) 
	$(RM) $(TARGET_ELF) 
	$(RM) $(TARGET_HEX)
