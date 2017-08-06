#include "cli.h"
#include "rtos.h"
#include "assert.h"
#include "print_log.h"
#include "etime.h"
#include <string.h>


LIST_HEAD(cli_head);

static void cli_task(void *arg);

e_err_t cli_register(struct cli_dev *cli)
{
    
    list_add(&cli_head, &cli->entry);
    
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
    const struct cli_operations *ops;
    ASSERT_PARAM(cli!=NULL);
    ASSERT_PARAM(cli->ops!=NULL);
    
    ops = cli->ops;
    
    if(ops->input==NULL)
    {
        LOG_ERROR("cli input is null!");
        return -E_ERROR;
    }
    
    
    return ops->input(cli);
}

e_size_t cli_puts(struct cli_dev *cli, char *str)
{
    const struct cli_operations *ops;
    ASSERT_PARAM(cli!=NULL);
    ASSERT_PARAM(cli->ops!=NULL);
    
    ops = cli->ops;
    
    if(ops->output==NULL)
    {
        LOG_ERROR("cli output is null!");
        return -E_ERROR;
    }
    
    e_size_t len = strlen(str);
    
    LOG_DEBUG("len=%d", len);
    
    return ops->output(cli, str, len);
}

e_err_t cli_execute(struct cli_dev *cli, struct cli_command *cmd)
{
    const struct cli_operations *ops;
    ASSERT_PARAM(cli!=NULL);
    ASSERT_PARAM(cli->ops!=NULL);
    ASSERT_PARAM(cmd!=NULL);
    ops = cli->ops;
    
    return ops->execute(cli, cmd);
}

e_err_t cli_parsing(struct cli_dev *cli,struct cli_command *cmd)
{
    const struct cli_operations *ops;
    ASSERT_PARAM(cli!=NULL);
    ASSERT_PARAM(cli->ops!=NULL);
    
    ops = cli->ops;
    if(ops->parsing==NULL)
    {
        LOG_ERROR("cli parsing is null!");
        return -E_ERROR;
    }
    
    return ops->parsing(cli, cmd);
}

void cli_polling(void)
{
    struct list_head *head;
    struct list_head *tnode = NULL;
    struct cli_command cmd;
    struct cli_dev *cli;
    
    
    list_for_each_safe(head, tnode, &cli_head)
    {
        cli = container_of(head, struct cli_dev, entry);
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