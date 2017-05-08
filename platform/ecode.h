#ifndef __ECODE_H__
#define __ECODE_H__
#include "config.h"
#include "includes.h"
#include "../targets/target.h"
#include "ecode_tick.h"
#include "serial.h"
#include "platform_driver.h"
#include "ecode_cli.h"
#include "timestamp.h"
#include "ecode_rtos.h"
#include "mcu.h"
#include "version.h"
#include "timer.h"
#include "pwm.h"

void ecode_init(void);

#endif