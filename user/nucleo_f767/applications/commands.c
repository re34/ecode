#include "commands.h"
#include <stdio.h>
#include <stdlib.h>
#include "ecode.h"

static e_err_t help(struct command_dev *command, char **args, int atgc);
static e_err_t version(struct command_dev *command, char **args, int argc);

static const struct command_item platform_commands[]={
    {.name="help", .handle = help, .brief="(), get the help info\r\n"},
    {.name="version", .handle=version, .brief = "(), print version information\r\n"},
    {NULL,NULL,NULL},
};

static struct command_list platform_commands_entry;


void command_register_platform_commands(void)
{
    command_register_commands(&platform_commands_entry, (struct command_item *)platform_commands);
}

static e_err_t help(struct command_dev *command, char **args, int argc)
{
    int i;
    if(argc==0)
    {
        command_print_help_list(command);
    }
    /*
    else
    {
        for(i=0;i<argc;i++)
        {
          if(cli_print_help(command, args[i])<0)
          {
            cli_error(command, ERROR_PARAM);
          }
        }
    }
    */
    
    //cli_error(command, ERROR_NONE);
    return E_EOK;
}

static e_err_t version(struct command_dev *command, char **args, int argc)
{
    //if(argc!=0)
    //    cli_error(command, ERROR_PARAM);
    
    FirmwareInfoPrint();
    
    //cli_error(command, ERROR_NONE);
    return E_EOK;
}
