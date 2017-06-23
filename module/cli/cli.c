#include "cli.h"
#include "rtos.h"


LIST_HEAD(cli_head);

static void cli_task(void *arg);

e_err_t cli_register(struct cli_dev *cli)
{
    
    list_add(&cli_head, &cli->entry;
    
    return E_EOK;
}

void cli_unregister(struct cli_dev *cli)
{
    if(cli==NULL)
        return;
    list_del(&cli->entry);
}

e_err_t cli_init(void)
{
    
    rtos_task_create(cli_task,"cli task", 512, NULL, 2, NULL);
    
    return E_EOK;
}

e_err_t cli_inputing(struct cli_dev *cli)
{
    ASSERT_PARAM(cli!=NULL);
    
    if(cli->input==NULL)
    {
        LOG_ERROR("cli input is null!");
        return -E_ERROR;
    }
    
    
    return cli->input(cli, cli->rx_buffer.buffer, cli->rx_buffer.size);
}

e_err_t cli_execute(struct cli_dev *cli, struct cli_command *cmd)
{
    ASSERT_PARAM(cli!=NULL);
    ASSERT_PARAM(cmd!=NULL);
    
    return cli->execute(cli, cmd);
}

e_err_t cli_parsing(struct cli_dev *cli,struct cli_command *cmd)
{
    ASSERT_PARAM(cli!=NULL);
    
    if(cli->parsing==NULL)
    {
        LOG_ERROR("cli parsing is null!");
        return -E_ERROR;
    }
    
    return cli->parsing(cli, cli->rx_buffer.buffer, cli->rx_buffer.size, cmd);
}

void cli_polling(void)
{
    struct list_head *cli;
    struct list_head *tnode = NULL;
    struct cli_command cmd;
    
    
    list_for_each_safe(cli, tnode, &cli_head)
    {
        if(cli_inputing(cli)!=E_EOK)
            continue;
        if(cli_parsing(cli, &cmd)!=E_EOK)
            continue;
        if(cli_execute(cli, &cmd)!=E_EOK)
        {
            
        }            
    }
}

static void cli_task(void *arg)
{

    while(1)
    {
        cli_polling();
        delay_ms(10);
    }
}