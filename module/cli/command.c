#include "command.h"
#include "link_list.h"
#include "cli.h"
#include "assert.h"
#include "print_log.h"
#include <string.h>
#include <ctype.h>

#define END_OF_LINE     '\n'

LIST_HEAD(command_head);

e_err_t command_input(struct cli_dev *cli)
{
    struct command_dev *command;
    const struct command_operations *ops;
    e_uint8_t *buffer;
    int data;
    ASSERT_PARAM(cli!=NULL);
    command = cli->private_data;
    ASSERT_PARAM(command->ops!=NULL);
    
    ops = command->ops;
    buffer = command->rx_buffer.buffer;
    
    while((data=ops->getc())!=-1)
    {
        if(command->rx_buffer.pos>command->rx_buffer.size)
        {
            LOG_DEBUG("command rx buffer is full!");
            return -E_EFULL;
        }
        buffer[command->rx_buffer.pos++]= data;
        
        ops->putc(data);
        
        if((data=='\b')&&(command->rx_buffer.pos!=0))
        {
            ops->putc(' ');
            ops->putc('\b');
            command->rx_buffer.pos--;
        }
        
        if(data==END_OF_LINE)
            return E_EOK;
    }
    
 
    return -E_EBUSY;
}

e_size_t command_puts(struct command_dev *command, const char *str)
{
    const struct command_operations *ops;
    e_uint8_t *pdata;
    e_size_t temp=0;
    e_size_t len;
    ASSERT_PARAM(command!=NULL);
    ASSERT_PARAM(command->ops!=NULL);
    ops = command->ops;
    
    pdata = (e_uint8_t *)str;
    len = strlen(str);
    temp = len;
    
    if(ops->putc==NULL)
        return -E_ERROR;
    
    while(len>0)
    {
        if(ops->putc(*pdata++)<0)
            break;
        len--;
    }
    
    return (len-temp);
}

e_size_t command_output(struct cli_dev *cli, void *buffer, e_size_t size)
{
    struct command_dev *command;
    const struct command_operations *ops;
    e_uint8_t *pdata;
    e_size_t pos=0;
    ASSERT_PARAM(cli!=NULL);
    command = cli->private_data;
    ASSERT_PARAM(command->ops!=NULL);
    ops = command->ops;
    
    pdata = buffer;
    
    if(ops->putc==NULL)
        return -E_ERROR;
    
    while(size>0)
    {
        if(ops->putc(*pdata++)<0)
            break;
        pos++;
    }
    
    return pos;
}

void command_print_promot(struct cli_dev *cli)
{
    cli_puts(cli, "ecode>>");
}

e_err_t command_parsing(struct cli_dev *cli, struct cli_command *cmd)
{
    struct command_dev *command;
    char *buffer;
    char *p;
    char *ptemp;
    e_size_t len;
    
    ASSERT_PARAM(cli!=NULL);
    
    command = cli->private_data;
    
    buffer = command->rx_buffer.buffer;
    len = command->rx_buffer.pos;
    if(len<=0)
        return -E_EEMPTY;
    
    p = buffer+command->rx_buffer.pos-1;
    
    while(p!=buffer)
    {
        if(isgraph(*p))
            break;
        p--;
        len--;
    }
    
    if(len==0)
        return -E_EEMPTY;
    
    buffer[len]='\0';
    
    p = buffer;
    
    while(p!=(buffer+len-1))
    {
        if(isgraph(*p))
            break;
        p++;
        len--;
    }
    
    if(len==0)
        return -E_EEMPTY;
    
    ptemp = strchr(p, '(');
    
    if(ptemp==NULL)
        return -E_ERROR;
    
    *ptemp = '\0';
    
    if(strlen(p)>=CMD_NAME_LEN)
    {
        LOG_DEBUG("cmd name is too long!");
        return -E_EFULL;
    }
    
    memcpy(command->params.name, p, strlen(p));
    command->params.name[strlen(p)]='\0';
    
    ptemp ++;
    p = ptemp;
    
    if(p==NULL)
    {
        LOG_DEBUG("no ) is found!");
        return -E_ERROR;
    }
    p = strchr(p, ')');
    if(p==NULL)
    {
        LOG_DEBUG("no ) is found!");
        return -E_ERROR;
    }
    
    *p = '\0';
    len = p - ptemp;
    if(len==0)
    {
        command->params.argc = 0;
    }
    else
    {
        command->params.argc = 1;
        command->params.args[0]=ptemp;
        
        p = strchr(ptemp, ',');
        
        while(p!=NULL)
        {
            *p++='\0';
            command->params.args[command->params.argc++]=p;
            if(command->params.argc>CMD_PARAM_NUM_MAX)
            {
                LOG_DEBUG("param is too long!");
                return -E_ERROR;
            }
            p = strchr(p, ',');
        }
    }
    
    cmd->cmd = command->params.name;
    cmd->arg = &command->params;
    command->rx_buffer.pos = 0;
    
    return E_EOK;
}

static struct command_item * command_match(const char *name)
{
    struct list_head *entry;
    struct command_list *command_list;
    struct command_item *item;
    
    list_for_each(entry, &command_head)
    {
        command_list = container_of(entry, struct command_list, entry);
        item = command_list->commands;
        if(item!=NULL)
        {
            while(item->name!=NULL)
            {
                if(0==strcmp(item->name, name))
                {
                    return item;
                }
                item++;
            }
        }
    }
    return NULL;
}

e_err_t command_execute(struct cli_dev *cli, struct cli_command *cmd)
{
    ASSERT_PARAM(cli!=NULL);
    ASSERT_PARAM(cmd!=NULL);
    struct command_params *params = cmd->arg;
    
    struct command_item *item;
    
    item = command_match(cmd->cmd);
    
    if((item!=NULL)&&(item->handle!=NULL))
    {
        if(item->handle(cli->private_data, params->args, params->argc)!=E_EOK)
        {
            
        }
    }
    else
    {
        
    }
    
    
    return E_EOK;
}

const struct cli_operations cli_operations={
    .input = command_input,
    .output = command_output,
    .parsing = command_parsing,
    .execute = command_execute,
};

e_err_t command_register(struct command_dev *command)
{
    ASSERT_PARAM(command!=NULL);
    command->cli.ops = &cli_operations;
    command->cli.private_data = command;
    
    return cli_register(&command->cli);
}

void command_register_commands(struct command_list *command_entry, struct command_item *items)
{
    command_entry->commands = items;
    list_add(&command_head, &command_entry->entry);
}

e_err_t command_print_help(struct command_dev *command, const char *name)
{
    struct command_item *item = NULL;
    
    item = command_match(name);
    if(item==NULL)
        return -E_ERROR;
    
    command_puts(command, item->name);
    command_puts(command, item->brief);
    
    return E_EOK;
}

e_err_t command_print_help_list(struct command_dev *command)
{
    struct list_head *entry;
    struct command_item *item = NULL;
    struct command_list *commands = NULL;
    
    command_puts(command, "\r\n**************help info***************\r\n");
    
    list_for_each(entry, &command_head)
    {
        commands = container_of(entry, struct command_list, entry);
        item = commands->commands;
        if(item!=NULL)
        {
            while(item->name!=NULL)
            {
                command_puts(command, item->name);
                command_puts(command, item->brief);
                item++;
            }
        }
        
    }
    
    command_puts(command, "\r\n***********help end*****************\r\n");
    
    return E_EOK;
}






