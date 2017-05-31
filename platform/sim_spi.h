#ifndef __SIM_SPI__
#define __SIM_SPI__

struct sim_spi_operations{
    void (*sck_set)(void);
    void (*sck_reset)(void);
    void (*mosi_set)(void);
    void (*mosi_reset)(void);
    int (*get_miso)(void);
};

int spi_sim_send_read_byte(struct sim_spi_operations *ops, int data);

#endif