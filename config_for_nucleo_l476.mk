
PROJECT     =ecode
TARGET_BOARD    =NUCLEO_L476
TARGET_CHIP     =STM32L4
STARTUP_FILE    =startup_stm32l476xx.s
LINK_FILE       =STM32L476RG_FLASH.ld
ARCH            =cortex-m4
FPU_FLAG        =-mfpu=fpv4-sp-d16 -mfloat-abi=softfp


DEFS            +=-D USE_HAL_DRIVER
DEFS            +=-D STM32L476xx

GCC_PREFIX      =arm-none-eabi-

OS_PORT_DIR     +=ARM_CM4F