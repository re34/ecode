
PROJECT     =ecode
TARGET_BOARD    =NUCLEO_F303
TARGET_CHIP     =STM32F3
STARTUP_FILE    =startup_stm32f303xe.s
LINK_FILE       =STM32F303XE_FLASH.ld
ARCH            =cortex-m4
FPU_FLAG        =-mfpu=fpv4-sp-d16 -mfloat-abi=softfp

DEFS            +=-D USE_HAL_DRIVER
DEFS            +=-D STM32F303xE

GCC_PREFIX      =arm-none-eabi-

OS_PORT_DIR     +=ARM_CM4F