#include "bt_cli_handle.h"
#include "ecode.h"
#include "bt_spp.h"
#include <string.h>

static int bt_mode(struct command_dev *command, char **args, int argc);
static int bt_write(struct command_dev *command, char **args, int argc);

static const struct command_item bt_commands[]={
    {.name="bt_mode", bt_mode, "(mode),mode=cli|com, set bt spp mode\r\n"},
    {.name="bt_write", bt_write, "(msg),msg type is string, write data to bt spp\r\n"},
    {NULL,NULL,NULL},
};

static struct command_list bt_commands_entry;


void cli_register_bt_commands(void)
{
    command_register_commands(&bt_commands_entry, bt_commands);
}

static int bt_mode(struct command_dev *command, char **args, int argc)
{
    if(argc!=1)
    {
        return ERROR_PARAM;
    }
    if(0==strcmp(args[0], "cli"))
    {
        bt_spp_set_mode(BT_CLI_MODE);
    }
    else if(0==strcmp(args[0], "com"))
    {
        bt_spp_set_mode(BT_COM_MODE);
    }
    else
        return ERROR_PARAM;
    
    return ERROR_NONE;
}

static int bt_write(struct command_dev *command, char **args, int argc)
{
    int len=0;
    if(argc!=1)
    {
        return ERROR_PARAM;
    }
    len = strlen(args[0]);
    bt_spp_write((unsigned char *)args[0], len);
    bt_spp_write("\r\n", 2);
    
    return ERROR_NONE;
}