#vpath %.s Drivers/CMSIS/Device/ST/STM32L0xx/Source/Templates/gcc
#vpath %.c Drivers/CMSIS/Device/ST/STM32L0xx/Source/Templates
#vpath %.c Drivers/STM32L0xx_HAL_Driver/Src
#vpath %.c Src
#vpath %.h Drivers/CMSIS/Include
#vpath %.h Drivers/CMSIS/Device/ST/STM32L0xx/Include
#vpath %.h Drivers/STM32L0xx_HAL_Driver/Inc
#vpath %.h Inc


PROJECT		=ecode
TARGET_CHIP	=STM32L0
TARGET_BOARD	=NUCLEO_L073
STARTUP_FILE	:=startup_stm32l073xx.S
LINK_FILE	:=STM32L073RZ_FLASH.ld


GCC_PREFIX	=arm-none-eabi-

DEFS += -D USE_HAL_DRIVER
DEFS += -D STM32L073xx

#TOP_DIR         =$(shell pwd)
TOP_DIR		=.
OBJ_DIR         =$(TOP_DIR)/Obj
OUTPUT_DIR      =$(TOP_DIR)/Output
SCRIPT_DIR      =$(TOP_DIR)/scripts

TARGET_CHIP_DIR :=$(TOP_DIR)/targets/TARGET_$(TARGET_CHIP)
TARGET_BOARD_DIR        :=$(TARGET_CHIP_DIR)/$(TARGET_BOARD)

TARGET_ELF	=$(OUTPUT_DIR)/$(PROJECT)_$(TARGET_BOARD).elf
TARGET_BIN	=$(OUTPUT_DIR)/$(PROJECT)_$(TARGET_BOARD).bin
TARGET_HEX	=$(OUTPUT_DIR)/$(PROJECT)_$(TARGET_BOARD).hex

LDSCRIPT        =$(TARGET_CHIP_DIR)/$(TARGET_BOARD)/TOOLCHAIN_GCC_ARM/$(LINK_FILE)

OBJCPFLAGS_ELF_TO_BIN	=-Obinary
OBJCPFLAGS_ELF_TO_HEX	=-Oihex
OBJCPFLAGS_BIN_TO_HEX	=-Ibinary -Oihex


INC_DIR		+=-I $(TOP_DIR)/targets
INC_DIR		+=-I $(TARGET_CHIP_DIR)
INC_DIR		+=-I $(TARGET_CHIP_DIR)/CMSIS
INC_DIR		+=-I $(TARGET_CHIP_DIR)/common
INC_DIR		+=-I $(TARGET_CHIP_DIR)/$(TARGET_BOARD)
INC_DIR		+=-I $(TARGET_CHIP_DIR)/BSP
INC_DIR		+=-I $(TARGET_CHIP_DIR)/$(TARGET_CHIP)xx_HAL_Driver/Inc
INC_DIR		+=-I $(TOP_DIR)/platform
INC_DIR		+=-I $(TOP_DIR)/user
INC_DIR		+=-I $(TOP_DIR)/drivers
INC_DIR		+=-I $(TOP_DIR)/common
#INC_DIR		+=-I $(TOP_DIR)/library/OS/FreeRTOS/include
#INC_DIR		+=-I $(TOP_DIR)/library/OS/FreeRTOS/portable/GCC/ARM_CM0
INC_DIR		+=-I $(TOP_DIR)/user/applications

CC		:=$(GCC_PREFIX)gcc
AS		:=$(GCC_PREFIX)as
LD		:=$(GCC_PREFIX)ld
AR		:=$(GCC_PREFIX)ar
OBJCP		:=$(GCC_PREFIX)objcopy
OBJSIZE		:=$(GCC_PREFIX)size

#CCFLAGS		+=-mcpu=cortex-m4 -mthumb -Wall -Os -ffunction-sections -fdata-sections -g -std=gnu99
CCFLAGS		+=-mcpu=cortex-m0plus -mthumb -Wall -Os -std=gnu99
CCFLAGS		+=-Wno-unused-variable	#don't warning unused variable
CCFLAGS		+=-nostartfiles
CCFLAGS		+=-ffunction-sections -fdata-sections
CCFLAGS		+=-g

LDFLAGS		+=-mcpu=cortex-m0plus -mthumb
LDFLAGS		+=-specs=nano.specs
LDFLAGS		+=-u_printf_float
LDFLAGS		+=-Wl,--gc-sections
#LDFLAGS		+=-mcpu=cortex-m4	-mthumb -Wall -Os -ffunction-sections -fdata-sections -g -std=gnu99

SOURCE		+=$(wildcard $(TARGET_CHIP_DIR)/*.c)
SOURCE		+=$(wildcard $(TARGET_CHIP_DIR)/CMSIS/*.c)
SOURCE		+=$(wildcard $(TARGET_CHIP_DIR)/common/*.c)
SOURCE		+=$(wildcard $(TARGET_CHIP_DIR)/$(TARGET_CHIP)xx_HAL_Driver/Src/*.c)
SOURCE		+=$(wildcard $(TOP_DIR)/platform/*.c)
SOURCE		+=$(wildcard $(TOP_DIR)/user/*.c)
SOURCE		+=$(wildcard $(TOP_DIR)/drivers/*.c)
SOURCE		+=$(wildcard $(TOP_DIR)/common/*.c)
SOURCE		+=$(wildcard $(TOP_DIR)/user/applications/*.c)
SOURCE		+=$(wildcard $(TARGET_CHIP_DIR)/BSP/*.c)
#SOURCE		+=$(wildcard $(TOP_DIR)/library/OS/FreeRTOS/*.c)
#SOURCE		+=$(wildcard $(TOP_DIR)/library/OS/FreeRTOS/portable/GCC/ARM_CM0/*.c)
#SOURCE		+=$(wildcard $(TOP_DIR)/library/OS/FreeRTOS/portable/MemMang/*4.c)
SOURCE_ASM	+=$(wildcard $(TARGET_CHIP_DIR)/$(TARGET_BOARD)/TOOLCHAIN_GCC_ARM/*.S)


C_OBJS		:=$(SOURCE:%.c=%.o)
ASM_OBJS	:=$(SOURCE_ASM:%.S=%.o)

.PHONY: all clean rebuild

all: $(TARGET_BIN) $(TARGET_ELF)


$(TARGET_BIN):$(TARGET_ELF)
	$(OBJCP) -O binary -S $< $@

$(TARGET_ELF):$(ASM_OBJS) $(C_OBJS)
	$(CC) $(LDFLAGS) $(addprefix $(OBJ_DIR)/,$(notdir $^)) -T $(LDSCRIPT) -o $@
	$(OBJSIZE) $@


$(C_OBJS):%.o:%.c
	$(CC) $(CCFLAGS) $(DEFS) $(INC_DIR) -c $^ -o  $(OBJ_DIR)/$(notdir $@)

$(ASM_OBJS):%.o:%.S
	rm -rf $(OBJ_DIR) $(OUTPUT_DIR)
	mkdir $(OBJ_DIR) $(OUTPUT_DIR)
	$(CC) $(CCFLAGS) -c $^ -o $(OBJ_DIR)/$(notdir $@)


clean:
	${RM} $(OBJ_DIR)/* $(TARGET_BIN) $(TARGET_ELF) $(TARGET_HEX)

