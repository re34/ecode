#include "bt_spp.h"
#include "ecode.h"
#include "bt_cli_handle.h"


struct bt_spp{
    bt_mode_t mode;
};


#define BT_RX_BUFFER_SIZE      100
static unsigned char bt_rx_buffer[BT_RX_BUFFER_SIZE];
static struct bt_spp bt_spp;

struct command_dev bt_cli_dev;

static void bt_spp_task(void *args);


static int bt_putc(char c)
{
    return serial_write(COM2, &c, 1);
}

static int bt_getc(void)
{
    int data;
    if(serial_read(COM2, &data, 1)>0)
        return data;
    else
        return -1;
}

static const struct command_operations command_ops = {
    .fgetc = bt_getc,
    .fputc = bt_putc,
};

static struct command_dev command_service = {
    .rx_buffer.buffer = bt_rx_buffer,
    .rx_buffer.size = BT_RX_BUFFER_SIZE,
    .ops = &command_ops,
};

void bt_spp_init(void)
{
    LOG_DEBUG("bt spp init...");

    
    //bt_spp.mode = BT_COM_MODE;
    bt_spp_set_mode(BT_CLI_MODE);
    //bt_spp_set_mode(BT_COM_MODE);
    cli_register_bt_commands();
    xTaskCreate(bt_spp_task,
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
        command_register(&bt_cli_dev);
    else
        command_unregister(&bt_cli_dev);
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
            ret = bt_spp_read(bt_rx_buffer, BT_RX_BUFFER_SIZE);
            if(ret<=0)
            {
                vTaskDelay(5);
                continue;
            }
            bt_rx_buffer[ret]='\0';
            print_log("%s", (char *)bt_rx_buffer);
        }
        else
        {
            vTaskDelay(10);
        }
    }
}




