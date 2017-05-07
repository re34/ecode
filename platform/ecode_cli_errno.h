#ifndef __ECODE_ERRNO_H__
#define __ECODE_ERRNO_H__


enum{
    ERROR_OK,
    ERROR_UNKNOW_CMD,
    ERROR_PARAM_LEN,
    ERROR_PARAM,
    ERROR_DOING,
    ERROR_BUSY,
    ERROR_UNKNOW,
    ERROR_NONE
};



const char *cli_match_error(int errno);

#endif