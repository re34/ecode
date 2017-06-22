#include "ecode_cli.h"
#include "cli_message_read_line.h"
#include "cli_message_parsing.h"
#include <string.h>
#include <stdarg.h>
#include "includes.h"

#define END_LINE    "\r\n"
#define DELIM_STR   ","


LIST_HEAD(cli_head);
LIST_HEAD(cli_commands_head);

void cli_device_register(struct cli_dev *dev, const char *name)
{
    dev->name = name;
    list_add(&cli_head, &dev->cli_entry);
}

void cli_device_unregister(struct cli_dev *dev)
{
    list_del(&dev->cli_entry);
}

void cli_register_commands(struct cli_commands_list * cli_commands_entry, const struct cli_command *command_list)
{
    cli_commands_entry->commands = (struct cli_command *)command_list;
    list_add(&cli_commands_head, &cli_commands_entry->entry);
}


int cli_puts(struct cli_dev *dev, const char *str)
{
    int ret;
    
    ret = stdio_puts(dev->stdio, str);
    
    return ret;
}


struct cli_command * cli_match_command(const char *name)
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

int cli_execute_command(struct cli_dev *dev, struct cli_command * command, struct cli_command_param *param)
{
    int ret = 0;
    
    ret = command->handle(dev, param->args, param->argc);
    
    return ret;
}

int cli_print_help(struct cli_dev *dev ,const char *name)
{
    struct cli_command *command = NULL;
    command = cli_match_command(name);
    if(command==NULL)
        return -1;
    cli_puts(dev, command->name);
    cli_puts(dev, command->help);
    
    return 0;
}

int cli_print_help_list(struct cli_dev *dev)
{
    struct list_head *commands;
    struct cli_command *command=NULL;
    
    cli_puts(dev, "\r\n\r\n***************help list******************\r\n\r\n");
    
    list_for_each(commands, &cli_commands_head)
    {
        command = ((struct cli_commands_list *)commands)->commands;
        if(command!=NULL)
        {
            while(command->name!=NULL)
            {
                cli_puts(dev, command->name);
                cli_puts(dev, command->help);
                command++;
            }

        }
    }
    
    cli_puts(dev, "\r\n***************help end*******************\r\n\r\n");

    return 0;
}

void cli_print_promot(struct cli_dev *dev)
{
    cli_puts(dev,"ecode>>>");
}

void cli_print_error(struct cli_dev *dev, int errno)
{
    if(errno==ERROR_NONE)
    {
        cli_print_promot(dev);
        return;
    }
        
    if(errno!=ERROR_OK)
      cli_puts(dev,"ERROR:");
    cli_puts(dev, cli_match_error(errno));
    cli_puts(dev, "\r\n");
    cli_print_promot(dev);
}

int cli_polling(void)
{
    int ret = -1;
    struct list_head *dev;
    struct list_head *tnode = NULL;
    struct cli_command_param param;
    struct cli_command *command=NULL;
    
    //list_for_each(dev, &cli_head)
    list_for_each_safe(dev,tnode, &cli_head)
    {
        if((((struct cli_dev *)dev)->flag&CLI_PROMOT)==0)
        {
            //stdio_puts(stdio_dev , "ecode>> ");
            cli_print_promot((struct cli_dev *)dev);
            ((struct cli_dev *)dev)->flag|=CLI_PROMOT;
        }
        if(cli_message_read_line((struct cli_dev *)dev, END_LINE)>0)
        {
            ret = 0;
            if(ecode_message_parsing(((struct cli_dev *)dev)->rxbuf, ((struct cli_dev *)dev)->rxlen, &param,DELIM_STR)==0)
            {
                command = cli_match_command(param.command_name);
                if(command!=NULL)
                {
                    if(cli_execute_command((struct cli_dev *)dev, command, &param)<0)
                    {
                        
                    }
                }
                else
                {
                    cli_print_error((struct cli_dev *)dev, ERROR_UNKNOW_CMD);
                }
            }
            else
            {
                cli_print_error((struct cli_dev *)dev, ERROR_UNKNOW_CMD);
            }
            ((struct cli_dev *)dev)->rxlen = 0;
        }
    }
    
    return ret;
}




