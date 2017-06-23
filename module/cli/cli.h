#ifndef __CLI_H__
#define __CLI_H__
#include "device.h"
#include "link_list.h"

struct cli_dev{
    struct list_head entry;
    const struct cli_operations *ops;
    void *private_data;
};

struct cli_command{
    char *cmd;
    void *arg;
};

struct cli_operations{
    e_err_t (*input)(struct cli_dev *cli);
    e_err_t (*parsing)(struct cli_dev *cli, struct cli_command *cmd);
    e_err_t (*execute)(struct cli_dev *cli, struct cli_command *cmd);
};

e_err_t cli_register(struct cli_dev *cli);
void cli_unregister(struct cli_dev *cli);
e_err_t cli_init(void);

#endif