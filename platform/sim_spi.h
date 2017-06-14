#ifndef __SIM_SPI__
#define __SIM_SPI__
#include "types.h"

struct sim_spi_operations{
    void (*data)
    void (*set_sck)(void *data, e_int32_t state);
    void (*set_mosi)(void *data, e_int32_t state);
    e_int32_t (*get_miso)(void *data);
    e_uint32_t delay_us;
    void (*udelay)(e_uint32_t us);
};



#endif