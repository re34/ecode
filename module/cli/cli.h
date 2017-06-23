#ifndef __CLI_H__
#define __CLI_H__
#include "device.h"
#include "link_list.h"

struct cli_dev{
    struct list_head entry;
    buffer_t tx_buffer;
    buffer_t rx_buffer;
    const struct cli_operations *ops;
};

struct cli_command{
    char *cmd;
    void *arg;
};

struct cli_operations{
    e_size_t (*input)(struct cli_dev *cli, void *buffer, e_size_t size);
    e_size_t (*output)(struct cli_dev *cli, void *buffer, e_size_t size);
    e_size_t (*parsing)(struct cli_dev *cli, void *buffer, e_size_t size, struct cli_command *param);
    e_size_t (*execute)(struct cli_dev *cli, struct cli_command *cmd);
};



#endif