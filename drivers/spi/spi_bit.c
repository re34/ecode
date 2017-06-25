#include "spi_bit.h"
#include "types.h"
#include "assert.h"

#define SET_SCK(ops, bit)    (ops->set_sck(ops->data, bit))
#define SET_MOSI(ops, bit)     (ops->set_mosi(ops->data, bit))
#define GET_MISO(ops)           (ops->get_miso(ops->data))

#define SCK_L(ops)          SET_SCK(ops, 0)
#define SCK_H(ops)          SET_SCK(ops, 1)
#define MOSI_L(ops)         SET_MOSI(ops, 0)
#define MOSI_H(ops)         SET_MOSI(ops, 1)



e_inline void spi_bit_udelay(const struct spi_bit_operations *ops)
{    
    if(ops->udelay!=NULL)
        ops->udelay(ops->delay_us);
}

int spi_bit_read_writeb(struct spi_dev *bus, e_uint8_t value)
{
    const struct spi_bit_operations *ops;
    struct spi_bit_dev *spi_bit_bus;
    spi_mode_t mode;
    e_uint8_t i;
    e_uint8_t data=0;
    ASSERT_PARAM(bus!=NULL);
    spi_bit_bus = container_of(bus, struct spi_bit_dev, parent);
    ASSERT_PARAM(spi_bit_bus->ops!=NULL);
    ops = spi_bit_bus->ops;
    
    mode = bus->mode;
    
    
    
    if(SPI_MODE0 == mode)
    {
        for(i=0;i<8;i++)
        {
            SCK_L(ops);
            if(value&0x80)
                MOSI_H(ops);
            else
                MOSI_L(ops);
            spi_bit_udelay(ops);
            SCK_H(ops);
            spi_bit_udelay(ops);
            data<<=1;
            if(GET_MISO(ops))
                data|=0x01;
            SCK_L(ops);
            value<<=1;
        }
    }else if(SPI_MODE1 == mode){
        for(i=0;i<8;i++)
        {
            SCK_L(ops);
            spi_bit_udelay(ops);
            SCK_H(ops); 
            spi_bit_udelay(ops);
            if(value&0x80)
                MOSI_H(ops);
            else
                MOSI_L(ops);
            SCK_L(ops);
            data<<=1;
            if(GET_MISO(ops))
                data|=0x01;
            value<<=1;
        }
    }else if(SPI_MODE2 == mode){
        for(i=0;i<8;i++)
        {
            SCK_H(ops);
            if(value&0x80)
                MOSI_H(ops);
            else
                MOSI_L(ops);
            spi_bit_udelay(ops);
            SCK_L(ops);
            spi_bit_udelay(ops);
            data<<=1;
            if(GET_MISO(ops))
                data|=0x01;
            SCK_H(ops);
            value<<=1;
        }
    }else if(SPI_MODE3 == mode){
        for(i=0;i<8;i++)
        {
            SCK_H(ops);
            spi_bit_udelay(ops);
            SCK_L(ops);
            if(value&0x80)
                MOSI_H(ops);
            else
                MOSI_L(ops);
            spi_bit_udelay(ops);
            SCK_H(ops);
            data<<=1;
            if(GET_MISO(ops))
                data|=0x01;
            value<<=1;
        }
    }
    
    return data;
}


e_err_t spi_bit_writeb(struct spi_dev *bus, e_uint8_t value)
{
    const struct spi_bit_operations *ops;
    struct spi_bit_dev *spi_bit_bus;
    spi_mode_t mode;
    e_uint8_t i;
    ASSERT_PARAM(bus!=NULL);
    spi_bit_bus = container_of(bus, struct spi_bit_dev, parent);
    ASSERT_PARAM(spi_bit_bus->ops!=NULL);
    ops = spi_bit_bus->ops;
    
    mode = bus->mode;
    
    
    
    if(SPI_MODE0 == mode)
    {
        for(i=0;i<8;i++)
        {
            SCK_L(ops);
            if(value&0x80)
                MOSI_H(ops);
            else
                MOSI_L(ops);
            spi_bit_udelay(ops);
            SCK_H(ops);
            spi_bit_udelay(ops);
            SCK_L(ops);
            value<<=1;
        }
    }else if(SPI_MODE1 == mode){
        for(i=0;i<8;i++)
        {
            SCK_L(ops);
            spi_bit_udelay(ops);
            SCK_H(ops); 
            spi_bit_udelay(ops);
            if(value&0x80)
                MOSI_H(ops);
            else
                MOSI_L(ops);
            SCK_L(ops);
            value<<=1;
        }
    }else if(SPI_MODE2 == mode){
        for(i=0;i<8;i++)
        {
            SCK_H(ops);
            if(value&0x80)
                MOSI_H(ops);
            else
                MOSI_L(ops);
            spi_bit_udelay(ops);
            SCK_L(ops);
            spi_bit_udelay(ops);
            SCK_H(ops);
            value<<=1;
        }
    }else if(SPI_MODE3 == mode){
        for(i=0;i<8;i++)
        {
            SCK_H(ops);
            spi_bit_udelay(ops);
            SCK_L(ops);
            if(value&0x80)
                MOSI_H(ops);
            else
                MOSI_L(ops);
            spi_bit_udelay(ops);
            SCK_H(ops);
            value<<=1;
        }
    }
    
    return E_EOK;
}

int spi_bit_readb(struct spi_dev *bus)
{
    const struct spi_bit_operations *ops;
    struct spi_bit_dev *spi_bit_bus;
    spi_mode_t mode;
    e_uint8_t i;
    e_uint8_t data=0;
    ASSERT_PARAM(bus!=NULL);
    spi_bit_bus = container_of(bus, struct spi_bit_dev, parent);
    ASSERT_PARAM(spi_bit_bus->ops!=NULL);
    ops = spi_bit_bus->ops;
    
    mode = bus->mode;
    
    if(SPI_MODE0 == mode)
    {
        for(i=0;i<8;i++)
        {
            SCK_L(ops);
            spi_bit_udelay(ops);
            SCK_H(ops);
            spi_bit_udelay(ops);
            data<<=1;
            if(GET_MISO(ops))
                data|=0x01;
            SCK_L(ops);
        }
    }else if(SPI_MODE1 == mode){
        for(i=0;i<8;i++)
        {
            SCK_L(ops);
            spi_bit_udelay(ops);
            SCK_H(ops); 
            spi_bit_udelay(ops);
            SCK_L(ops);
            data<<=1;
            if(GET_MISO(ops))
                data|=0x01;
        }
    }else if(SPI_MODE2 == mode){
        for(i=0;i<8;i++)
        {
            SCK_H(ops);
            spi_bit_udelay(ops);
            SCK_L(ops);
            spi_bit_udelay(ops);
            data<<=1;
            if(GET_MISO(ops))
                data|=0x01;
            SCK_H(ops);
        }
    }else if(SPI_MODE3 == mode){
        for(i=0;i<8;i++)
        {
            SCK_H(ops);
            spi_bit_udelay(ops);
            SCK_L(ops);
            spi_bit_udelay(ops);
            SCK_H(ops);
            data<<=1;
            if(GET_MISO(ops))
                data|=0x01;
        }
    }
    
    return data;
}


static const struct spi_operations spi_ops={
    .readb = spi_bit_readb,
    .writeb = spi_bit_writeb,
    .read_writeb = spi_bit_read_writeb,
};

e_err_t spi_bit_register(int bus, struct spi_bit_dev *spi_bit_bus)
{
    ASSERT_PARAM(spi_bit_bus!=NULL);
    
    spi_bit_bus->parent.ops = &spi_ops;
    
    return spi_register(bus, &spi_bit_bus->parent);
}
