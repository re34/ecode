#include "smart_car.h"
#include "ecode.h"
#include "motor.h"
#include "smart_car_handle.h"
#include "encoder.h"



struct smart_car{
    int left_speed;
    int right_speed;
};

#define SPEED_STEP      5

struct smart_car smart_car={0};

static struct pid left_pid;
static struct pid right_pid;


static void smart_car_task(void *args);

int smart_car_init()
{
    cli_register_smart_car_commands();
    smart_car_set_speed(0,0);
    encoder_init();
    pid_init(&left_pid);
    pid_init(&right_pid);
    rtos_task_create(smart_car_task,
                "smart_car_task",
                512,
                NULL,
                3,
                NULL);
    return 0;
}

int smart_car_get_left_speed()
{
    return smart_car.left_speed;
}

int smart_car_get_right_speed()
{
    return smart_car.right_speed;
}


void smart_car_set_speed(int left_speed, int right_speed)
{
    motor_dir_t left_dir=MOTOR_DIR_FORWARD;
    motor_dir_t right_dir=MOTOR_DIR_FORWARD;
    
    
    smart_car.left_speed = left_speed;
    smart_car.right_speed = right_speed;
    
    if(left_speed<0)
    {
        left_dir = MOTOR_DIR_BACK;
        left_speed = -left_speed;
    }
    if(right_speed<0)
    {
        right_dir = MOTOR_DIR_BACK;
        right_speed = -right_speed;
    }
    
    motor_set_left_duty(left_speed, left_dir);
    motor_set_right_duty(right_speed, right_dir);
}


void smart_car_speed_up()
{
    int left_speed;
    int right_speed;
    motor_dir_t left_dir = MOTOR_DIR_FORWARD;
    motor_dir_t right_dir = MOTOR_DIR_FORWARD;
    
    left_speed = smart_car.left_speed;
    right_speed = smart_car.right_speed;
    
    left_speed += SPEED_STEP;
    right_speed += SPEED_STEP;
    if(left_speed<100)
    {
        smart_car.left_speed = left_speed;
        if(left_speed < 0)
        {
            left_dir = MOTOR_DIR_BACK;
            left_speed = -left_speed;
        }

        motor_set_left_duty(left_speed, left_dir);
    }
    
    if(right_speed<100)
    {
        smart_car.right_speed = right_speed;
        if(right_speed < 0)
        {
            right_dir = MOTOR_DIR_BACK;
            right_speed = -right_speed;
        }
        motor_set_right_duty(right_speed, right_dir);
    } 
    
}

void smart_car_speed_down()
{
    int left_speed;
    int right_speed;
    motor_dir_t left_dir = MOTOR_DIR_FORWARD;
    motor_dir_t right_dir = MOTOR_DIR_FORWARD;
    
    left_speed = smart_car.left_speed;
    right_speed = smart_car.right_speed;
    
    left_speed -= SPEED_STEP;
    right_speed -= SPEED_STEP;

    if(left_speed>-100)
    {
        smart_car.left_speed = left_speed;
        if(left_speed < 0)
        {
            left_dir = MOTOR_DIR_BACK;
            left_speed = -left_speed;
        }

        motor_set_left_duty(left_speed, left_dir);
    }
    
    if(right_speed>-100)
    {
        smart_car.right_speed = right_speed;
        if(right_speed < 0)
        {
            right_dir = MOTOR_DIR_BACK;
            right_speed = -right_speed;
        }
        motor_set_right_duty(right_speed, right_dir);
    } 
}

void smart_car_turn_left()
{
    int right_speed;
    motor_dir_t right_dir = MOTOR_DIR_FORWARD;

    right_speed = smart_car.right_speed;

    if(right_speed<0)
    {
        right_dir = MOTOR_DIR_BACK;
        right_speed -= SPEED_STEP;
        if(right_speed<=-100)
            return;
        smart_car.right_speed = right_speed;
        right_speed = -right_speed;
    }
    else
    {
        right_dir = MOTOR_DIR_FORWARD;
        right_speed += SPEED_STEP;
        if(right_speed>=100)
            return;
        smart_car.right_speed = right_speed;
    }
    
    motor_set_right_duty(right_speed, right_dir);  
}

void smart_car_turn_right()
{
    int left_speed;
    motor_dir_t left_dir = MOTOR_DIR_FORWARD;
    
    left_speed = smart_car.left_speed;
    
    if(left_speed<0)
    {
        left_dir = MOTOR_DIR_BACK;
        left_speed -= SPEED_STEP;
        if(left_speed<=-100)
            return;
        smart_car.left_speed = left_speed;
        left_speed = -left_speed;
    }
    else
    {
        left_dir = MOTOR_DIR_FORWARD;
        left_speed += SPEED_STEP;
        if(left_speed>=100)
            return;
        smart_car.left_speed = left_speed;
    }
    
    motor_set_left_duty(left_speed, left_dir);  
}

static void smart_car_task(void *args)
{
    struct encoder encoder;
    int left_duty;
    int right_duty;
    motor_dir_t left_dir;
    motor_dir_t right_dir;
    
    while(1)
    {
        encoder_get(&encoder);
        left_duty = pid_increment_calc(&left_pid, encoder.left_count);
        right_duty = pid_increment_calc(&right_pid, encoder.right_count);
        
        if(left_duty<0)
            left_dir = MOTOR_DIR_BACK;
        else
            left_dir = MOTOR_DIR_FORWARD;
        if(right_duty<0)
            right_dir = MOTOR_DIR_BACK;
        else
            right_dir = MOTOR_DIR_FORWARD;
        
        //motor_set_left_duty(left_duty, left_dir); 
        //motor_set_right_duty(right_duty, right_dir);
        delay_ms(50);
    }
}