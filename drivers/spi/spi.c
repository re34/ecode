#include "spi.h"
#include "assert.h"

#ifndef SPIn
#define SPIn       2
#endif


static struct spi_dev *_spis[SPIn];

e_err_t spi_write(int bus, void *buffer, e_size_t size)
{
    struct spi_dev *spi_bus;
    const struct spi_operations *ops;
    e_base_t i;
    e_uint8_t *pdata = buffer;
    
    ASSERT_PARAM(bus<SPIn);
    spi_bus = _spis[bus];
    ASSERT_PARAM(spi_bus!=NULL);
    
    ops = spi_bus->ops;
    
    if(ops->writeb==NULL)
        return E_EOK;
    
    if(spi_bus->dir&SPI_DIR_TX!=SPI_DIR_TX)
        return -E_ERROR;
    
    for(i=0;i<size;i++)
    {
        if(ops->writeb(spi_bus, pdata[i])!=E_EOK)
            return -E_ERROR;
    }
    
    return E_EOK;
}

e_err_t spi_read(int bus, void *buffer, e_size_t size)
{
    struct spi_dev *spi_bus;
    const struct spi_operations *ops;
    e_base_t i;
    e_uint8_t *pdata = buffer;
    
    ASSERT_PARAM(bus<SPIn);
    spi_bus = _spis[bus];
    ASSERT_PARAM(spi_bus!=NULL);
    
    ops = spi_bus->ops;
    
    if(ops->writeb==NULL)
        return E_EOK;
    
    if(spi_bus->dir&SPI_DIR_RX!=SPI_DIR_RX)
        return -E_ERROR;
    
    for(i=0;i<size;i++)
    {
        pdata[i]=ops->readb(spi_bus);
    }
    
    return E_EOK;
}

e_err_t spi_read_write(int bus, void *txbuffer, void *rxbuffer, e_size_t size)
{
    struct spi_dev *spi_bus;
    const struct spi_operations *ops;
    e_base_t i;
    e_uint8_t *ptxdata = txbuffer;
    e_uint8_t *prxdata = rxbuffer;
    
    ASSERT_PARAM(bus<SPIn);
    spi_bus = _spis[bus];
    ASSERT_PARAM(spi_bus!=NULL);
    
    ops = spi_bus->ops;
    
    if(ops->read_writeb==NULL)
        return E_EOK;
    
    if(spi_bus->dir&SPI_DIR_RX!=SPI_DIR_DUPLEX)
        return -E_ERROR;
    
    for(i=0;i<size;i++)
    {
        prxdata[i]=ops->read_writeb(spi_bus, ptxdata[i]);
    }
    
    return E_EOK;
}

e_err_t spi_register(int bus, struct spi_dev *spi_bus)
{
    
    ASSERT_PARAM(bus<SPIn);
    ASSERT_PARAM(spi_bus!=NULL);
    
    _spis[bus]=spi_bus;

    return E_EOK;
}


