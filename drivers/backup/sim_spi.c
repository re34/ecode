#include "sim_spi.h"


#define spi_sim_delay()     //ecode_tick_delay_us(5)


#define SET_SCK(ops, val)   ops->set_sck(ops->data, val)
#define SET_MOSI(ops, val)  ops->set_mosi(ops->data, val)
#define GET_MISO(ops, val)  ops->get_miso(ops->data);


e_inline void sim_spi_delay(struct sim_spi_operations *ops)
{
    ops->udelay(ops->delay_us);
}


#define SCK_H(ops)  SET_SCK(ops,1)
#define SCK_L(ops)  SET_SCK(ops,0)
#define MOSI_H(ops) SET_MOSI(ops,1)
#define MOSI_L(ops) SET_MOSI(ops,0)

int spi_sim_send_read_byte(struct sim_spi_operations *ops, int data)
{
    e_uint8_t i;
    int ret;
    
    for(i=0;i<8;i++)
    {
        SCK_L(ops);
        if(data&0x80)
            MOSI_H(ops);
        else
            MOSI_L(ops);
        sim_spi_delay();
        ret<<=1;
        SCK_H(ops);
        if(ops->get_miso()==1)
            ret |= 0x01;
        data<<=1;
        sim_spi_delay();
        SCK_L(ops);
        
    }
    
    return ret;
}