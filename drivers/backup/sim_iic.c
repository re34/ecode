#include "sim_iic.h"
#include "iic.h"


void sim_iic_delay(int delay)
{
    
}

int sim_iic_start(struct sim_iic_operations *ops)
{
    int timeout = 0xff;
    
    ops->sda_set();
    ops->scl_set();
    sim_iic_delay(5);
    
    while(ops->get_sda()==0)
    {
        timeout--;
        if(timeout==0)
            return -1;
    }
    ops->sda_reset();
    sim_iic_delay(2);
    ops->sda_reset();
    sim_iic_delay(5);
    
    return 0;
}

int sim_iic_stop(struct sim_iic_operations *ops)
{
    ops->sda_reset();
    sim_iic_delay(4);
    ops->scl_set();
    sim_iic_delay(4);
    ops->sda_set();
    sim_iic_delay(5);
    
    return 0;
}

int sim_iic_write(struct sim_iic_operations *ops, UInt8 data, UInt8 ack)
{
    int i;
    for(i=0;i<8;i++)
    {
        ops->scl_reset();
        if(data&0x80)
            ops->sda_set();
        else
            ops->sda_reset();
        sim_iic_delay(2);
        ops->scl_set();
        sim_iic_delay(4);
        ops->scl_reset();
        sim_iic_delay(2);
        data<<=1;
    }
    
    if(ack==IIC_ACK)
    {
        ops->sda_reset();
        sim_iic_delay(4);
        ops->scl_set();
        sim_iic_delay(4);
        ops->scl_reset();
        sim_iic_delay(4);
    }
    else
    {
        ops->sda_set();
        sim_iic_delay(4);
        ops->scl_set();
        sim_iic_delay(4);
        ops->scl_reset();
        sim_iic_delay(4);
    }
    
    return 0;
}

int sim_iic_read(struct sim_iic_operations *ops)
{
    int data = 0;
    int i;
    
    ops->sda_set();
    ops->scl_reset();
    sim_iic_delay(4);
    for(i=0;i<8;i++)
    {
        data<<=1;
        ops->scl_set();
        sim_iic_delay(4);
        if(ops->get_sda()==1)
            data|=0x01;
        ops->scl_reset();
        sim_iic_delay(4);
        
    }
    
    return data;
}

int sim_iic_read_ack(struct sim_iic_operations *ops)
{
    int err_timer = 0;
    
    ops->sda_set();
    sim_iic_delay(1);
    ops->scl_set();
    sim_iic_delay(2);
    
    while(ops->get_sda()==1)
    {
        err_timer++;
        if(err_timer>250)
        {
            ops->scl_reset();
            goto error;
        }
        
    }
    sim_iic_delay(4);
    ops->scl_reset();
    sim_iic_delay(4);
    
    return 0;
error:
    sim_iic_stop(ops);
    return -1;
}

/*
int sim_iic_ack(struct sim_iic_operations *ops, UInt8 ack)
{
    ops->sda_reset();
    sim_iic_delay(4);
    ops->scl_set();
    sim_iic_delay(4);
    ops->scl_reset();
    sim_iic_delay(4);
    
    return 0;
}*/

/*
int sim_iic_nack(struct sim_iic_operations *ops)
{
    ops->sda_set();
    sim_iic_delay(4);
    
    ops->scl_set();
    sim_iic_delay(4);
    
    ops->scl_reset();
    sim_iic_delay(4);
    
    return 0;
}
*/
/*
int sim_iic_wait_ack(struct sim_iic_operations *ops)
{
    int err_timer = 0;
    
    ops->sda_set();
    sim_iic_delay(1);
    ops->scl_set();
    sim_iic_delay(2);
    
    while(ops->get_sda()==1)
    {
        err_timer++;
        if(err_timer>250)
        {
            ops->scl_reset();
            goto error;
        }
        
    }
    sim_iic_delay(4);
    ops->scl_reset();
    sim_iic_delay(4);
    
    return 0;
error:
    sim_iic_stop(ops);
    return -1;
}
*/