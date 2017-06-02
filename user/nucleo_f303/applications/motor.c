#include "motor.h"
#include "ecode.h"
#include "board.h"

#define IN1 PB_2
#define IN2 PB_1
#define IN3 PB_15
#define IN4 PB_14





void motor_init()
{
    stm_pin_init(IN1, PIN_MODE_OUTPUT_PP, PIN_PUPD_UP);
    stm_pin_init(IN2, PIN_MODE_OUTPUT_PP, PIN_PUPD_UP);
    stm_pin_init(IN3, PIN_MODE_OUTPUT_PP, PIN_PUPD_UP);
    stm_pin_init(IN4, PIN_MODE_OUTPUT_PP, PIN_PUPD_UP);
    
    pwm_period_us(PWM2, 100);
    pwm_period_us(PWM3, 100);
    pwm_write(PWM2, 0);
    pwm_write(PWM3, 0);
}

void motor_brake()
{
    stm_pin_reset(IN1);
    stm_pin_reset(IN2);
    stm_pin_reset(IN3);
    stm_pin_reset(IN4);
    pwm_write(PWM2, 0);
    pwm_write(PWM3, 0);
}

void motor_set_left_duty(int duty, motor_dir_t dir)
{
    if(dir==MOTOR_DIR_FORWARD)
    {
        stm_pin_reset(IN1);
        stm_pin_set(IN2);
    }else if(dir==MOTOR_DIR_BACK){
        stm_pin_set(IN1);
        stm_pin_reset(IN2);
    }
    LOG_DEBUG("left duty=%d, dir=%s", duty, ((dir==MOTOR_DIR_FORWARD)?"forward":"back"));
    pwm_write(PWM2, duty/100.0f);
}

void motor_set_right_duty(int duty, motor_dir_t dir)
{  
    if(dir==MOTOR_DIR_FORWARD)
    {
        stm_pin_set(IN3);
        stm_pin_reset(IN4);
    }else if(dir==MOTOR_DIR_BACK){
        stm_pin_reset(IN3);
        stm_pin_set(IN4);
    }
    LOG_DEBUG("right duty=%d, dir=%s", duty, ((dir==MOTOR_DIR_FORWARD)?"forward":"back"));
    pwm_write(PWM3, duty/100.0f);
}

