#ifndef __CLI_MESSAGE_PARSING_H__
#define __CLI_MESSAGE_PARSING_H__
#include "ecode_cli.h"

int ecode_message_parsing(char *message, int message_len,struct cli_command_param *param,char *delim);
#endif