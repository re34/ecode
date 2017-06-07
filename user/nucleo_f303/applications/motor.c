#include "motor.h"
#include "ecode.h"
#include "board.h"

#define IN1 PB_2
#define IN2 PB_1
#define IN3 PB_15
#define IN4 PB_14





void motor_init()
{
    pin_mode(IN1, PIN_MODE_OUTPUT_PP, PIN_PUPD_UP);
    pin_mode(IN2, PIN_MODE_OUTPUT_PP, PIN_PUPD_UP);
    pin_mode(IN3, PIN_MODE_OUTPUT_PP, PIN_PUPD_UP);
    pin_mode(IN4, PIN_MODE_OUTPUT_PP, PIN_PUPD_UP);
    
    pwm_period_us(PWM2, 100);
    pwm_period_us(PWM3, 100);
    pwm_write(PWM2, 0);
    pwm_write(PWM3, 0);
}

void motor_brake()
{
    pin_write(IN1, PIN_LOW);
    pin_write(IN2, PIN_LOW);
    pin_write(IN3, PIN_LOW);
    pin_write(IN4, PIN_LOW);
    pwm_write(PWM2, 0);
    pwm_write(PWM3, 0);
}

void motor_set_left_duty(int duty, motor_dir_t dir)
{
    if(dir==MOTOR_DIR_FORWARD)
    {
        pin_write(IN1, PIN_LOW);
        pin_write(IN2, PIN_HIGH);
    }else if(dir==MOTOR_DIR_BACK){
        pin_write(IN1, PIN_HIGH);
        pin_write(IN2, PIN_LOW);
    }
    LOG_DEBUG("left duty=%d, dir=%s", duty, ((dir==MOTOR_DIR_FORWARD)?"forward":"back"));
    pwm_write(PWM2, duty/100.0f);
}

void motor_set_right_duty(int duty, motor_dir_t dir)
{  
    if(dir==MOTOR_DIR_FORWARD)
    {
        pin_write(IN3, PIN_HIGH);
        pin_write(IN4, PIN_LOW);
    }else if(dir==MOTOR_DIR_BACK){
        pin_write(IN3, PIN_LOW);
        pin_write(IN4, PIN_HIGH);
    }
    LOG_DEBUG("right duty=%d, dir=%s", duty, ((dir==MOTOR_DIR_FORWARD)?"forward":"back"));
    pwm_write(PWM3, duty/100.0f);
}

