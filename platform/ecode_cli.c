#include "ecode_cli.h"
#include "ecode_message_read_line.h"
#include "ecode_message_parsing.h"
#include <string.h>
#include <stdarg.h>
#include "includes.h"

#define END_LINE    "\r\n"
#define DELIM_STR   ","


LIST_HEAD(ecode_cli_head);
LIST_HEAD(cli_commands_head);

void ecode_register_cli_device(struct ecode_cli_dev *dev, const char *name)
{
    dev->name = name;
    list_add(&ecode_cli_head, &dev->cli_entry);
    
}


void ecode_register_commands(struct cli_commands_list * cli_commands_entry, const struct cli_command *command_list)
{
    cli_commands_entry->commands = (struct cli_command *)command_list;
    list_add(&cli_commands_head, &cli_commands_entry->entry);
}


int ecode_cli_puts(struct ecode_cli_dev *dev, const char *str)
{
    int ret;
    
    ret = stdio_puts(dev->stdio, str);
    
    return ret;
}


struct cli_command * ecode_cli_match_command(const char *name)
{
    struct list_head *commands;
    struct cli_command *command=NULL;
    
    list_for_each(commands, &cli_commands_head)
    {
        command = ((struct cli_commands_list *)commands)->commands;
        if(command!=NULL)
        {
            while(command->name!=NULL)
            {
                if(0==strcmp(command->name, name))
                {
                    return command;
                }
                command++;
            }

        }
    }
    return NULL;
}

int ecode_cli_execute_command(struct ecode_cli_dev *dev, struct cli_command * command, struct cli_command_param *param)
{
    int ret = 0;
    
    ret = command->handle(dev, param->args, param->argc);
    
    return ret;
}

int ecode_cli_print_help(struct ecode_cli_dev *dev ,const char *name)
{
    struct cli_command *command = NULL;
    command = ecode_cli_match_command(name);
    if(command==NULL)
        return -1;
    ecode_cli_puts(dev, command->name);
    ecode_cli_puts(dev, command->help);
    
    return 0;
}

int ecode_cli_print_help_list(struct ecode_cli_dev *dev)
{
    struct list_head *commands;
    struct cli_command *command=NULL;
    
    ecode_cli_puts(dev, "\r\n\r\n***************help list******************\r\n\r\n");
    
    list_for_each(commands, &cli_commands_head)
    {
        command = ((struct cli_commands_list *)commands)->commands;
        if(command!=NULL)
        {
            while(command->name!=NULL)
            {
                ecode_cli_puts(dev, command->name);
                ecode_cli_puts(dev, command->help);
                command++;
            }

        }
    }
    
    ecode_cli_puts(dev, "\r\n***************help end*******************\r\n\r\n");

    return 0;
}


void cli_print_error(struct ecode_cli_dev *dev, int errno)
{
    if(errno==ERROR_NONE)
        return;
    if(errno!=ERROR_OK)
      ecode_cli_puts(dev,"ERROR:");
    ecode_cli_puts(dev, cli_match_error(errno));
    ecode_cli_puts(dev, "\r\n");
}

int ecode_cli_polling(void)
{
    struct list_head *dev;
    struct cli_command_param param;
    struct cli_command *command=NULL;
    
    list_for_each(dev, &ecode_cli_head)
    {
        
        if(ecode_message_read_line((struct ecode_cli_dev *)dev, END_LINE)>0)
        {
            if(ecode_message_parsing(((struct ecode_cli_dev *)dev)->rxbuf, ((struct ecode_cli_dev *)dev)->rxlen, &param,DELIM_STR)==0)
            {
                command = ecode_cli_match_command(param.command_name);
                if(command!=NULL)
                {
                    if(ecode_cli_execute_command((struct ecode_cli_dev *)dev, command, &param)<0)
                    {
                        
                    }
                }
                else
                {
                    cli_print_error((struct ecode_cli_dev *)dev, ERROR_UNKNOW_CMD);
                }
            }
            else
            {
                cli_print_error((struct ecode_cli_dev *)dev, ERROR_UNKNOW_CMD);
            }
            ((struct ecode_cli_dev *)dev)->rxlen = 0;
        }
    }
    
    return 0;
}




