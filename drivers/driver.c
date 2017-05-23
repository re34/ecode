#include "driver.h"
#include "ecode.h"
#include "uart.h"
#include "uart2.h"
#include "serial_platform_driver.h"

static struct timer_device base_timer;
static struct pwm_device pwm_dev;
static struct pwm_operations pwm_ops;


int driver_init(void)
{
    struct print_log_interface fprint_log;
    
    uart_init();
    fprint_log.put_char = bsp_uart_writebyte;
    print_log_register_io(fprint_log);
    
    if(uart_init()<0)
    {
        LOG_ERROR("uart init failed!");
    }
    if(uart2_init()<0)
    {
        LOG_ERROR("uart2 init failed");
    }
    if(serial_platform_init()<0)
    {
        LOG_ERROR("serial platform driver init failed!");
    }
    //register base timer
    base_timer.init = bsp_base_timer_init;
    base_timer.start = bsp_base_timer_start;
    base_timer.stop = bsp_base_timer_stop;
    base_timer.reset = bsp_base_timer_reset;
    base_timer.read = bsp_base_timer_read;
    base_timer.read_ms = bsp_base_timer_read_ms;
    base_timer.read_us = bsp_base_timer_read_us;
    if(timer_register(TIMER1, &base_timer)<0)
    {
        LOG_ERROR("TIMER1 register error!!!");
    }
    
    pwm_ops.init = bsp_pwm_init;
    pwm_ops.period_us = bsp_pwm_period_us;
    pwm_ops.pulsewidth_us = bsp_pwm_pulsewidth_us;
    
    pwm_dev.ops = &pwm_ops;
    
    if(pwm_register(PWM1, &pwm_dev)<0)
    {
        LOG_ERROR("PWM1 register error!!!");
    } 
}

