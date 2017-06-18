#include "encoder.h"
#include "board.h"

#define SPEED_PERIOD        100//ms
#define WHEEL_DIAMETER      66//mm
#define ENCODER_NUM         30
#define PI              3.1415926f

static struct encoder _encoder;

static void exti4_callback(void);
static void exti15_10_callback(void);


void encoder_init(void)
{
    exti4_init(exti4_callback);
    exti15_10_init(exti15_10_callback);
    _encoder.left_count = 0;
    _encoder.right_count = 0;
    LOG_DEBUG("encoder module is inited!");
}


void encoder_get(struct encoder *encoder)
{
    encoder->left_count = _encoder.left_count;
    encoder->right_count = _encoder.right_count;
    _encoder.left_count = 0;
    _encoder.right_count = 0;
}

static void exti4_callback(void)
{
    _encoder.left_count++;
}

static void exti15_10_callback(void)
{
    _encoder.right_count++;    
}