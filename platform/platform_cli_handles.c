#include "platform_cli_handles.h"
#include "ecode_cli.h"
#include "mcu.h"
#include "ecode_tick.h"
#include "timestamp.h"
#include <stdio.h>
#include <stdlib.h>

static int help(struct ecode_cli_dev *dev, char **args, int atgc);
static int reset(struct ecode_cli_dev *dev, char **args, int argc);
static int get_sys_time(struct ecode_cli_dev *dev, char **args, int argc);

static const struct cli_command platform_commands[]={
    {.name="help", help, "(), get the help info\r\n"},
    {.name="reset", reset, "(), reset the system\r\n"},
    {.name="gettime", get_sys_time, "(), get system time from start up\r\n"},
    {NULL,NULL,NULL},
};

static struct cli_commands_list platform_commands_entry;


void cli_register_platform_commands(void)
{
    ecode_register_commands(&platform_commands_entry, platform_commands);
}

static int help(struct ecode_cli_dev *dev, char **args, int argc)
{
    int i;
    if(argc==0)
    {
        ecode_cli_print_help_list(dev);
    }
    else
    {
        for(i=0;i<argc;i++)
        {
          if(ecode_cli_print_help(dev, args[i])<0)
          {
            cli_error(dev, ERROR_PARAM);
          }
        }
    }
    
    cli_error(dev, ERROR_NONE);
}

static int reset(struct ecode_cli_dev *dev, char **args, int argc)
{
    
    if(argc!=0)
        cli_error(dev, ERROR_PARAM);
    
    mcu_reset();
    
    ecode_tick_delay_ms(50);
    cli_error(dev, ERROR_NONE);
}

static int get_sys_time(struct ecode_cli_dev *dev, char **args, int argc)
{
    struct timestamp timestamp;
    if(argc!=0)
        cli_error(dev, ERROR_NONE);
    
    timestamp = get_timestamp();

    ecode_cli_print(dev, "time:%d s %d ms\r\n", timestamp.second, timestamp.msecond);
    
    cli_error(dev, ERROR_NONE);
}