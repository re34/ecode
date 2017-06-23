#include "command.h"

#define END_OF_LINE     '\n'
e_err_t command_input(struct cli_dev *cli)
{
    struct command_dev *command;
    struct command_operations *ops;
    e_uint8_t *buffer;
    int data;
    ASSERT_PARAM(cli!=NULL);
    command = cli->private_data;
    ASSERT_PARAM(command->ops!=NULL);
    
    buffer = command->rx_buffer.buffer;
    
    while((data=ops->getc())!=-1)
    {
        if(command->rx_buffer.pos<command->rx_buffer.size)
        {
            LOG_DEBUG("command rx buffer is full!");
            return -E_EFULL;
        }
        buffer[command->rx_buffer.pos++]= data;
        ops->putc(data);
    }
    
    if(data==END_OF_LINE)
        return E_EOK;
    
    return -E_EBUSY;
}

e_err_t command_parsing(struct cli_dev *cli, struct cli_command *cmd)
{
    struct command_dev *command;
    e_uint8_t *buffer;
    char *p;
    char *ptemp;
    e_size_t len;
    
    ASSERT_PARAM(cli!=NULL);
    
    buffer = command->rx_buffer.buffer;
    len = command->rx_buffer.pos;
    if(len<=0)
        return -E_EEMPTY;
    
    p = buffer+pos;
    
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
    
    while(p!=(buffer+len))
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
    
    memcpy(command->cmd.name, p, strlen(p));
    
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
        command->cmd.argc = 0;
    }
    else
    {
        cmd->cmd.argc = 1;
        command->cmd.args[0]=ptemp;
        
        p = strchr(ptemp, ',');
        
        while(p!=NULL)
        {
            *p++='\0';
            command->cmd.args[command->cmd.argc++]=p;
            if(command->cmd.argc>CMD_PARAM_NUM_MAX)
            {
                LOG_DEBUG("param is too long!");
                return -E_ERROR;
            }
            p = strchr(p, ',');
        }
    }
    
    cmd->cmd = command->cmd.name;
    cmd->arg = command->cmd.args;
    
    return E_EOK;
}

e_err_t command_execute(struct cli_dev *cli, struct cli_command *cmd)
{
    
    
    return E_EOK;
}

const struct cli_operations={
    .input = command_input,
    .parsing = command_parsing,
    .execute = command_execute,
};

e_err_t command_register(struct command_dev *command)
{
    ASSERT_PARAM(command!=NULL);
    command->cli.ops = cli_operations;
    command->private_data = command;
    
    return cli_register(&command->cli);
}






