#ifndef __ECODE_CLI_H__
#define __ECODE_CLI_H__
#include "includes.h"
#include "stdio_ex.h"
#include "ecode_cli.h"
#include "cli_errno.h"


#define CLI_BUF_SIZE    512
#define CLI_MAX_PARAM    100

#define CLI_PROMOT      1

struct cli_dev{
    struct list_head cli_entry;
    const char *name;
    int rxlen;
    char rxbuf[CLI_BUF_SIZE];
    struct stdioex_device *stdio;
    int flag;
    void *private_data;
};

typedef int (*cli_command_handle_t)(struct cli_dev *dev,char **args, int argc);

struct cli_command_param{
    char *command_name;
    char *args[CLI_MAX_PARAM];
    int argc;
};

struct cli_command{
    const char *name;
    const cli_command_handle_t handle;
    const char *help;
};

struct cli_commands_list{
    struct list_head entry;
    struct cli_command *commands;
};

void cli_print_error(struct cli_dev *dev, int errno);

#define cli_error(dev, errno)   \
            do{cli_print_error(dev, errno);return (-errno);}while(0)

                
#define cli_print(dev, fmt, ...)  stdio_print(dev->stdio, fmt, ##__VA_ARGS__)


void cli_device_register(struct cli_dev *dev, const char *name);
void cli_device_unregister(struct cli_dev *dev);
int cli_puts(struct cli_dev *dev, const char *str);
void cli_register_commands(struct cli_commands_list * cli_commands_entry, const struct cli_command *command_list);
int cli_print_help(struct cli_dev *dev ,const char *name);
int cli_print_help_list(struct cli_dev *dev);
void cli_print_promot(struct cli_dev *dev);
void cli_print_error(struct cli_dev *dev, int errno);
int cli_polling(void);

#endif