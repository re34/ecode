#ifndef __COMMAND_H__
#define __COMMAND_H__

#define CMD_NAME_LEN        25
#define CMD_PARAM_NUM_MAX   50

struct command_dev{
    struct cli_dev cli;
    buffer_t rx_buffer;
    buffer_t tx_buffer;
    struct command_params params;
    const command_operations *ops;
};

struct command_operations{
    int (*getc)(void);
    int (*putc)(char c);
};

struct command_params{
    char name[CMD_NAME_LEN];
    char *args[CMD_PARAM_NUM_MAX];
    int argc;
};

typedef e_err_t (*command_handle_t)(struct command_dev *command, char **args, int argc);

struct command_item{
    const char name[CMD_NAME_LEN];
    command_handle_t handle;
    const char *brief;
};

struct command_list{
    struct list_head entry;
    struct command_item *commands;
}

e_err_t command_register(struct command_dev *command);
void command_register_commands(struct command_list *command_entry, struct command_item *items);
#endif