#ifndef __CONFIG_H__
#define __CONFIG_H__
#include "ecode_config.h"


#ifndef RTOS_EN
#define RTOS_EN     1
#endif

#ifndef CONFIG_USE_LWIP
#define CONSIG_USE_LWIP	0
#endif

#ifndef CONFIG_USE_ASSERT
#define CONFIG_USE_ASSERT   1
#endif

#ifndef CONFIG_USE_BOOTLOADER
#define CONFIG_USE_BOOTLOADER   0
#endif

#ifndef CONFIG_BOOTLOADER_EN
#define CONFIG_BOOTLOADER_EN    0
#endif

#endif