#ifndef __COMMAND_H__
#define __COMMAND_H__

#define CMD_NAME_LEN        25
#define CMD_PARAM_NUM_MAX   50

struct command_dev{
    struct cli_dev cli;
    buffer_t rx_buffer;
    buffer_t tx_buffer;
    struct command cmd;
    const command_operations *ops;
};

struct command_operations{
    int (*getc)(void);
    int (*putc)(char c);
};

struct command{
    char name[CMD_NAME_LEN];
    char *args[CMD_PARAM_NUM_MAX];
    int argc;
};


#endif