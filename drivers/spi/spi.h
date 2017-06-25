#ifndef __SPI_H__
#define __SPI_H__
#include "types.h"



typedef enum{
    SPI_MODE0,
    SPI_MODE1,
    SPI_MODE2,
    SPI_MODE3,
}spi_mode_t;

#define SPI_DIR_RX          (1<<0)
#define SPI_DIR_TX          (1<<1)
#define SPI_DIR_DUPLEX      (SPI_DIR_RX|SPI_DIR_TX)



struct spi_dev{
    spi_mode_t mode;
    e_uint8_t dir;
    const struct spi_operations *ops;
};

struct spi_operations{
    int (*readb)(struct spi_dev *bus);
    e_err_t (*writeb)(struct spi_dev *bus, e_uint8_t value);
    int (*read_writeb)(struct spi_dev *bus, e_uint8_t value);
};


e_err_t spi_write(int bus, void *buffer, e_size_t size);
e_err_t spi_read(int bus, void *buffer, e_size_t size);
e_err_t spi_read_write(int bus, void *txbuffer, void *rxbuffer, e_size_t size);
e_err_t spi_register(int bus, struct spi_dev *spi_bus);
#endif