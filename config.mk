
PROJECT     =ecode
TARGET_BOARD    =NUCLEO_L073
TARGET_CHIP     =STM32L0
STARTUP_FILE    =startup_stm32l073xx.s
LINK_FILE       =STM32L073RZ_FLASH.ld
ARCH            =cortex-m0plus
FPU_FLAG        =

DEFS            +=-D USE_HAL_DRIVER
DEFS            +=-D STM32L073xx

GCC_PREFIX      =arm-none-eabi-

OS_PORT_DIR     +=ARM_CM0
