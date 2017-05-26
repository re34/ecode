
include config.mk

#PROJECT_DIR         =$(shell pwd)
PROJECT_DIR		=.
TARGET_CHIP_DIR =$(PROJECT_DIR)/targets/TARGET_$(TARGET_CHIP)
OBJ_DIR         =$(PROJECT_DIR)/Obj
OUTPUT_DIR      =$(PROJECT_DIR)/Output
SCRIPT_DIR      =$(PROJECT_DIR)/scripts


TARGET_ELF	=$(OUTPUT_DIR)/$(PROJECT)_$(TARGET_BOARD).elf
TARGET_BIN	=$(OUTPUT_DIR)/$(PROJECT)_$(TARGET_BOARD).bin
TARGET_HEX	=$(OUTPUT_DIR)/$(PROJECT)_$(TARGET_BOARD).hex

LDSCRIPT        =$(TARGET_CHIP_DIR)/$(TARGET_BOARD)/TOOLCHAIN_GCC_ARM/$(LINK_FILE)

OBJCPFLAGS_ELF_TO_BIN	=-Obinary
OBJCPFLAGS_ELF_TO_HEX	=-Oihex
OBJCPFLAGS_BIN_TO_HEX	=-Ibinary -Oihex


INC_DIR		+=-I $(PROJECT_DIR)/targets
INC_DIR		+=-I $(TARGET_CHIP_DIR)
INC_DIR		+=-I $(TARGET_CHIP_DIR)/CMSIS
INC_DIR		+=-I $(TARGET_CHIP_DIR)/common
INC_DIR		+=-I $(TARGET_CHIP_DIR)/$(TARGET_BOARD)
INC_DIR		+=-I $(TARGET_CHIP_DIR)/BSP
INC_DIR		+=-I $(TARGET_CHIP_DIR)/$(TARGET_CHIP)xx_HAL_Driver/Inc
INC_DIR		+=-I $(PROJECT_DIR)/platform
INC_DIR		+=-I $(PROJECT_DIR)/user
INC_DIR		+=-I $(PROJECT_DIR)/drivers
INC_DIR		+=-I $(PROJECT_DIR)/common
INC_DIR		+=-I $(PROJECT_DIR)/library/OS/FreeRTOS/include
INC_DIR		+=-I $(PROJECT_DIR)/user/applications
INC_DIR		+=-I $(PROJECT_DIR)/library/OS/FreeRTOS/portable/GCC/$(OS_PORT_DIR)

CC		:=$(GCC_PREFIX)gcc
AS		:=$(GCC_PREFIX)as
LD		:=$(GCC_PREFIX)ld
AR		:=$(GCC_PREFIX)ar
OBJCP		:=$(GCC_PREFIX)objcopy
OBJSIZE		:=$(GCC_PREFIX)size

CCFLAGS		+=-mcpu=$(ARCH) -mthumb -Wall -Os -std=gnu99
CCFLAGS		+=$(FPU_FLAG)
CCFLAGS		+=-Wno-unused-variable	#don't warning unused variable
CCFLAGS		+=-nostartfiles
CCFLAGS		+=-ffunction-sections -fdata-sections
CCFLAGS		+=-g

LDFLAGS		+=-mcpu=$(ARCH) -mthumb
#LDFLAGS		+=-specs=nano.specs
LDFLAGS		+=-u_printf_float
LDFLAGS		+=-Wl,--gc-sections
LDFLAGS		+=$(FPU_FLAG)


SOURCE		+=$(wildcard $(TARGET_CHIP_DIR)/*.c)
SOURCE		+=$(wildcard $(TARGET_CHIP_DIR)/CMSIS/*.c)
SOURCE		+=$(wildcard $(TARGET_CHIP_DIR)/common/*.c)
SOURCE		+=$(wildcard $(TARGET_CHIP_DIR)/$(TARGET_CHIP)xx_HAL_Driver/Src/*.c)
SOURCE		+=$(wildcard $(PROJECT_DIR)/platform/*.c)
SOURCE		+=$(wildcard $(PROJECT_DIR)/user/*.c)
SOURCE		+=$(wildcard $(PROJECT_DIR)/drivers/*.c)
SOURCE		+=$(wildcard $(PROJECT_DIR)/common/*.c)
SOURCE		+=$(wildcard $(PROJECT_DIR)/user/applications/*.c)
SOURCE		+=$(wildcard $(TARGET_CHIP_DIR)/BSP/*.c)
SOURCE		+=$(wildcard $(PROJECT_DIR)/library/OS/FreeRTOS/*.c)
SOURCE		+=$(wildcard $(PROJECT_DIR)/library/OS/FreeRTOS/portable/MemMang/*4.c)
SOURCE		+=$(wildcard $(PROJECT_DIR)/library/OS/FreeRTOS/portable/GCC/$(OS_PORT_DIR)/*.c)
SOURCE_ASM	+=$(wildcard $(TARGET_CHIP_DIR)/$(TARGET_BOARD)/TOOLCHAIN_GCC_ARM/*.s)


C_OBJS		:=$(SOURCE:%.c=%.o)
ASM_OBJS	:=$(SOURCE_ASM:%.s=%.o)

.PHONY: all clean rebuild

all: $(TARGET_BIN) $(TARGET_ELF)


$(TARGET_BIN):$(TARGET_ELF)
	$(OBJCP) -O binary -S $< $@

$(TARGET_ELF):$(ASM_OBJS) $(C_OBJS)
	$(CC) $(LDFLAGS) $(addprefix $(OBJ_DIR)/,$(notdir $^)) -T $(LDSCRIPT) -o $@
	$(OBJSIZE) $@


$(C_OBJS):%.o:%.c
	$(CC) $(CCFLAGS) $(DEFS) $(INC_DIR) -c $^ -o  $(OBJ_DIR)/$(notdir $@)

$(ASM_OBJS):%.o:%.s
	rm -rf $(OBJ_DIR) $(OUTPUT_DIR)
	mkdir $(OBJ_DIR) $(OUTPUT_DIR)
	$(CC) $(CCFLAGS) -c $^ -o $(OBJ_DIR)/$(notdir $@)


clean:
	${RM} $(OBJ_DIR)/* $(TARGET_BIN) $(TARGET_ELF) $(TARGET_HEX)

