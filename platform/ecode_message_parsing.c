#include "ecode_message_parsing.h"
#include <string.h>



int ecode_message_parsing(char *message, int message_len,struct cli_command_param *param,char *delim)
{
    char *p = NULL;
    char  *msg = message;
    char *parg = NULL;
    
    msg[message_len]='\0';
    
    p = strchr(msg, '(');
    if(p==NULL)
        return -1;
    *p++ = '\0';
    if(p==NULL)
        return -2;
    parg = p;
    
    p = strchr(p, ')');
    
    if(p==NULL)
        return -3;
    
    *p = '\0';
    
    param->command_name = msg;
    

    if(p==parg)
        param->argc=0;
    else
    {
        param->argc=1;
        param->args[0]=parg;
        
        p = strstr(parg, delim);
        
        while(p!=NULL)
        {
            *p++='\0';
            param->args[param->argc++]=p;
            if(param->argc>CLI_MAX_PARAM)
            {
                return -4;
            }
            p = strstr(p, delim);
            
        }
    }
    
    return 0;
}



