#ifndef __SPI_BIT_H__
#define __SPI_BIT_H__
#include "spi.h"


struct spi_bit_dev{
    struct spi_dev parent;
    const struct spi_bit_operations *ops;
};

struct spi_bit_operations{
    void (*set_sck)(void *data, int bit);
    void (*set_mosi)(void *data, int bit);
    int (*get_miso)(void *data);
    void *data;
    e_uint32_t delay_us;
    void (*udelay)(e_uint32_t us);
};

int spi_bit_read_writeb(struct spi_dev *bus, e_uint8_t value);
e_err_t spi_bit_writeb(struct spi_dev *bus, e_uint8_t value);
int spi_bit_readb(struct spi_dev *bus);
e_err_t spi_bit_register(int bus, struct spi_bit_dev *spi_bit_bus);
#endif