#include "command_handle.h"
#include <stdio.h>
#include <stdlib.h>
#include "ecode.h"

static int help(struct command_dev *command, char **args, int argc);
static int version(struct command_dev *command, char **args, int argc);
static int reboot(struct command_dev *command, char **args, int argc);
static int update(struct command_dev *command, char **args, int argc);

static const struct command_item platform_commands[]={
    {.name="help", .handle = help, .brief="(), get the help info\r\n"},
    {.name="version", .handle=version, .brief = "(), print version information\r\n"},
    {.name="reboot", .handle=reboot, .brief="(),reboot the system\r\n"},
    {.name="update", .handle=update, .brief="(), upgrade firmware\r\n"},
    {NULL,NULL,NULL},
};

static struct command_list platform_commands_entry;


void command_register_platform_commands(void)
{
    command_register_commands(&platform_commands_entry, (struct command_item *)platform_commands);
}

static int help(struct command_dev *command, char **args, int argc)
{
    int i;
    if(argc==0)
    {
        command_print_help_list(command);
    }
    else
    {
        for(i=0;i<argc;i++)
        {
          if(command_print_help(command, args[i])<0)
          {
            return ERROR_PARAM;
          }
        }
    }
    
    return ERROR_NONE;
}

static int version(struct command_dev *command, char **args, int argc)
{
    if(argc!=0)
        return ERROR_PARAM;
    
    FirmwareInfoPrint();
    
    return ERROR_NONE;
}

static int reboot(struct command_dev *command, char **args, int argc)
{
    if(argc!=0)
        return ERROR_PARAM;
    
    cli_puts(&command->cli, "system will reboot!!!\r\n");
    
    NVIC_SystemReset();    
    
    return ERROR_NONE;
}
static int update(struct command_dev *command, char **args, int argc)
{
    if(argc!=0)
        return ERROR_PARAM;
    
    if(firmware_upgrade()!=E_EOK)
    {
        cli_puts(&command->cli, "firmware update failed!\r\n");
    }
    
    
    return ERROR_NONE;
}
