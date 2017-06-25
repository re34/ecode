#include "bt_spp.h"
#include "ecode.h"
#include "bt_cli_handle.h"


struct bt_spp{
    bt_mode_t mode;
};


#define BT_BUFFER_SIZE      100
static unsigned char bt_buffer[BT_BUFFER_SIZE];
static struct bt_spp bt_spp;


static void bt_spp_task(void *args);


static int bt_putc(char c)
{
    return serial_write(COM2, (char *)&c, 1);
}

static int bt_getc()
{
    int data;
    
    if(serial_read(COM2,&data, 1)>0)
        return data;
    
    return -1;
}

static const struct command_operations bt_commands_ops={
    .getc = bt_getc,
    .putc = bt_putc,
};

struct command_dev bt_command_dev={
    .rx_buffer.buffer = bt_buffer,
    .rx_buffer.size = BT_BUFFER_SIZE,
    .rx_buffer.pos = 0,
    .ops = &bt_commands_ops,
};

void bt_spp_init(void)
{
    LOG_DEBUG("bt spp init...");
    
    //bt_spp.mode = BT_COM_MODE;
    bt_spp_set_mode(BT_CLI_MODE);
    //bt_spp_set_mode(BT_COM_MODE);
    cli_register_bt_commands();
    rtos_task_create(bt_spp_task,
                "bt_spp_task",
                512,
                NULL,
                3,
                NULL);
}

void bt_spp_set_mode(bt_mode_t mode)
{   
    if(bt_spp.mode==mode)
        return;
    bt_spp.mode = mode;
    
    if(mode==BT_CLI_MODE)
        command_register(&bt_command_dev);
    else
        command_unregister(&bt_command_dev);
    LOG_DEBUG("bt mode changed");
}


int bt_spp_write(unsigned char *buf, int len)
{
    return serial_write(COM2, (char *)buf, len);
}

int bt_spp_read(unsigned char *buf, int len)
{
    int size;
    
    size = serial_read(COM2, (char *)buf, len);
    
    return size;
}

static void bt_spp_task(void *args)
{
    int ret=0;
    LOG_DEBUG("bt spp task running...");
    
    while(1)
    {
        if(bt_spp.mode==BT_COM_MODE)
        {
            ret = bt_spp_read(bt_buffer, BT_BUFFER_SIZE);
            if(ret<=0)
            {
                delay_ms(5);
                continue;
            }
            bt_buffer[ret]='\0';
            print_log("%s", (char *)bt_buffer);
        }
        else
        {
            delay_ms(10);
        }
    }
}




