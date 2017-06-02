#ifndef __MOTOR_H__
#define __MOTOR_H__

typedef enum{
    MOTOR_DIR_FORWARD,
    MOTOR_DIR_BACK,
}motor_dir_t;


void motor_init();
void motor_brake();
void motor_set_left_duty(int duty, motor_dir_t dir);
void motor_set_right_duty(int duty, motor_dir_t dir);

#endif