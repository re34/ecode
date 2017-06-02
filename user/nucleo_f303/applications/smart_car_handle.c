#include "smart_car_handle.h"
#include "motor.h"
#include "smart_car.h"
#include <stdlib.h>
#include "ecode.h"
#include <string.h>


static int speed(struct ecode_cli_dev *dev, char **args, int argc);
static int speed_up(struct ecode_cli_dev *dev, char **args, int argc);
static int speed_down(struct ecode_cli_dev *dev, char **args, int argc);
static int turn_left(struct ecode_cli_dev *dev, char **args, int argc);
static int turn_right(struct ecode_cli_dev *dev, char **args, int argc);


static const struct cli_command smart_car_commands[]={
    {.name="speed",speed, "({-l/-r,}speed), paramter one select left or right wheel, paramter two is the speed\r\n" },
    {.name="speed up", speed_up, "(), speed up\r\n"},
    {.name="speed down", speed_down, "(), speed down\r\n"},
    {.name="turn left", turn_left, "(), turn to the left\r\n"},
    {.name="turn right", turn_right, "(), turn to thre right\r\n"},
    {NULL, NULL, NULL},
};

static struct cli_commands_list smart_car_commands_entry;

void cli_register_smart_car_commands()
{
    ecode_register_commands(&smart_car_commands_entry, smart_car_commands);
} 

static int speed(struct ecode_cli_dev *dev, char **args, int argc)
{
    int left_speed;
    int right_speed;
    
    
    if(argc==1)
    {
        left_speed = atoi(args[0]);
        smart_car_set_speed(left_speed, left_speed);
    }else if(argc==2){
        if(0==strcmp(args[0], "-l")){
            left_speed = atoi(args[1]);
            right_speed = smart_car_get_right_speed();
            smart_car_set_speed(left_speed, right_speed);
        }else if(0==strcmp(args[0], "-r")){
            right_speed = atoi(args[1]);
            left_speed = smart_car_get_left_speed();
            smart_car_set_speed(left_speed, right_speed);
        }else{
            cli_error(dev, ERROR_PARAM);
        }
    }
    else
        cli_error(dev, ERROR_PARAM);
    
    
    cli_error(dev, ERROR_NONE);
}

static int speed_up(struct ecode_cli_dev *dev, char **args, int argc)
{
    if(argc!=0)
        cli_error(dev, ERROR_PARAM);
    
    smart_car_speed_up();
    
    cli_error(dev, ERROR_NONE);
}

static int speed_down(struct ecode_cli_dev *dev, char **args, int argc)
{
    if(argc!=0)
        cli_error(dev, ERROR_PARAM);
    
    smart_car_speed_down();
    
    cli_error(dev, ERROR_NONE);
}


static int turn_left(struct ecode_cli_dev *dev, char **args, int argc)
{
    if(argc!=0)
        cli_error(dev, ERROR_PARAM);
    
    smart_car_turn_left();
    
    cli_error(dev, ERROR_NONE);
}

static int turn_right(struct ecode_cli_dev *dev, char **args, int argc)
{
    if(argc!=0)
        cli_error(dev, ERROR_PARAM);
    
    smart_car_turn_right();
    
    cli_error(dev, ERROR_NONE);
}