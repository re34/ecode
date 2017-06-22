#ifndef __CLI_H__
#define __CLI_H__
#include "device.h"

struct cli_dev{
    struct device dev;
    buffer_t tx_buffer;
    buffer_t rx_buffer;
    const struct cli_operations *ops;
};

struct cli_operations{
    e_size_t (*input)(struct cli_dev *cli);
    e_size_t (*parsing)(struct cli_dev *cli, void *buffer, e_size_t size);
};

#endif