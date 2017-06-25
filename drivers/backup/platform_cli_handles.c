#include "platform_cli_handles.h"
#include "ecode_cli.h"
#include "timestamp.h"
#include <stdio.h>
#include <stdlib.h>
#include "ecode.h"

static int help(struct cli_dev *dev, char **args, int atgc);
static int get_sys_time(struct cli_dev *dev, char **args, int argc);
static int get_version(struct cli_dev *dev, char **args, int argc);
static int ls(struct cli_dev *dev, char **args, int argc);

static const struct cli_command platform_commands[]={
    {.name="help", help, "(), get the help info\r\n"},
    {.name="time", get_sys_time, "(), get system time from start up\r\n"},
    {.name="version", get_version, "(), print version information\r\n"},
    {.name="ls", ls, "(), list file or folder content\r\n"},
    {NULL,NULL,NULL},
};

static struct cli_commands_list platform_commands_entry;


void cli_register_platform_commands(void)
{
    cli_register_commands(&platform_commands_entry, platform_commands);
}

static int help(struct cli_dev *dev, char **args, int argc)
{
    int i;
    if(argc==0)
    {
        cli_print_help_list(dev);
    }
    else
    {
        for(i=0;i<argc;i++)
        {
          if(cli_print_help(dev, args[i])<0)
          {
            cli_error(dev, ERROR_PARAM);
          }
        }
    }
    
    cli_error(dev, ERROR_NONE);
}


static int get_sys_time(struct cli_dev *dev, char **args, int argc)
{
    struct timestamp timestamp;
    if(argc!=0)
        cli_error(dev, ERROR_NONE);
    
    timestamp = get_timestamp();

    cli_print(dev, "time:%d s %d ms\r\n", timestamp.second, timestamp.msecond);
    
    cli_error(dev, ERROR_NONE);
}

static int get_version(struct cli_dev *dev, char **args, int argc)
{
    if(argc!=0)
        cli_error(dev, ERROR_PARAM);
    
    FirmwareInfoPrint();
    
    cli_error(dev, ERROR_NONE);
}

static int ls(struct cli_dev *dev, char **args, int argc)
{
   



    cli_error(dev, ERROR_NONE);
}