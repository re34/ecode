#include "speed.h"
#include "board.h"

#define SPEED_PERIOD        100//ms
#define WHEEL_DIAMETER      66//mm
#define ENCODER_NUM         30
#define PI              3.1415926f

struct speed{
    int left_counter;
    int right_counter;
    float left_speed;
    float right_speed;
    float speed;
};

static struct speed speed={
    .left_counter = 0,
    .right_counter = 0,
    .left_speed = 0,
    .right_speed = 0,
    .speed = 0,    
};

static struct timeout timeout;

static void speed_period(void);
static void exti4_callback(void);
static void exti15_10_callback(void);


void speed_init(void)
{
    timeout_attach(&timeout, speed_period, SPEED_PERIOD);
    exti4_init(exti4_callback);
    exti15_10_init(exti15_10_callback);
    
    LOG_DEBUG("speed module is inited!");
}


float speed_read(void)
{
    //待实现
    return speed.speed;
}

float speed_read_left(void)
{
    return speed.left_speed;
}

float speed_read_right(void)
{
    return speed.right_speed;
}

static void speed_period(void)
{
    int right_counter = 0;
    int left_counter = 0;
    
    right_counter = speed.right_counter;
    left_counter = speed.left_counter;
    speed.right_counter = 0;
    speed.left_counter = 0;
    
    speed.right_speed = right_counter/ENCODER_NUM*PI*WHEEL_DIAMETER/SPEED_PERIOD;//m/s
    speed.left_speed = left_counter/ENCODER_NUM*PI*WHEEL_DIAMETER/SPEED_PERIOD;
}

static void exti4_callback(void)
{
    speed.left_counter++;
}

static void exti15_10_callback(void)
{
    speed.right_counter++;    
}