#include "cli_errno.h"


const char *errors[]={
    "OK",
    "Unknow command",
    "Argumment list length is error",
    "Argumeants is error",
    "Bad processing",
    "Device or resource is busy",
    "Unknow error",
};


const char *cli_match_error(int errno)
{
    if(errno<sizeof(errors)/sizeof(errors[0]))
    {
        return errors[errno];  
    }
    else
    {
        return errors[ERROR_UNKNOW];
    }
    
}
        
