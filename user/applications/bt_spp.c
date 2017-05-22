#include "bt_spp.h"
#include "ecode.h"
#include "bt_cli_handle.h"


struct bt_spp{
    bt_mode_t mode;
};


#define BT_BUFFER_SIZE      100
static unsigned char bt_buffer[BT_BUFFER_SIZE];
static struct bt_spp bt_spp;

struct ecode_cli_dev bt_cli_dev;
static struct stdioex_device bt_stdio;

static void bt_spp_task(void *args);


static int bt_putc(unsigned char c)
{
    return serial_write(COM2, &c, 1);
}

static int bt_getc()
{
    int data;
    
    data = serial_in_waiting(COM2);
    if(data<=0)
        return 0;
    
    if(serial_read(COM2, &data,1)<=0)
        return 0;
    return data;
}


void bt_spp_init(void)
{
    
    bt_stdio.put_char = bt_putc;
    bt_stdio.get_char = bt_getc;
    bt_cli_dev.stdio = &bt_stdio;
    
    bt_spp.mode = BT_COM_MODE;
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
        ecode_register_cli_device(&bt_cli_dev, "BT CLI");
    else
        ecode_unregister_cli_device(&bt_cli_dev);
    LOG_DEBUG("bt mode changed");
}


int bt_spp_write(unsigned char *buf, int len)
{
    return serial_write(COM2, (char *)buf, len);
}

int bt_spp_read(unsigned char *buf, int len)
{
    int ret = 0;
    ret = serial_in_waiting(COM2);
    if(ret>0)
    {
        len = ((ret>len)?len:ret);
        ret = serial_read(COM2, (char *)buf, len);
    }
        
    return ret;
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
                vTaskDelay(5);
                continue;
            }
            bt_buffer[ret]='\0';
            print_log("%s", (char *)bt_buffer);
        }
        else
        {
            vTaskDelay(10);
        }
    }
}




