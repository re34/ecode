#include "sim_spi.h"
#include "../common/includes.h"

#define spi_sim_delay()     ecode_tick_delay_us(5)

int spi_sim_send_read_byte(struct sim_spi_operations *ops, int data)
{
    UInt8 i;
    int ret;
    
    for(i=0;i<8;i++)
    {
        ops->sck_reset();
        if(data&0x80)
            ops->mosi_set();
        else
            ops->mosi_reset();
        spi_sim_delay();
        ret<<=1;
        ops->sck_set();
        if(ops->get_miso()==1)
            ret |= 0x01;
        data<<=1;
        spi_sim_delay();
        ops->sck_reset();
        
    }
    
    return ret;
}