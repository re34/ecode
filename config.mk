
PROJECT     =ecode
TARGET_BOARD    =NUCLEO_F767
TARGET_CHIP     =STM32F7
STARTUP_FILE    =startup_stm32f767xx.
LINK_FILE       =STM32F767ZI_FLASH.ld
ARCH            =cortex-m7
FPU_FLAG        =-mfpu=fpv5-sp-d16 -mfloat-abi=softfp

DEFS            +=-D USE_HAL_DRIVER
DEFS            +=-D STM32F767xx

GCC_PREFIX      =arm-none-eabi-

OS_PORT_DIR     +=ARM_CM7/r0p1
