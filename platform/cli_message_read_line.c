#include "cli_message_read_line.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "stdio_ex.h"


int ecode_message_read_line(struct ecode_cli_dev *dev, const char *end)
{
    char data;
    char *msg;
    int i;
    int end_len = strlen(end);
    int match_len = 0;
    int ret=0;
    
    struct stdioex_device * stdio_dev;
    
     stdio_dev = dev->stdio;
    
    if((dev->flag&CLI_PROMOT)==0)
    {
        stdio_puts(stdio_dev , "ecode>> ");
        dev->flag|=CLI_PROMOT;
    }
    
    data = stdio_dev->get_char();
    
    if(data<=0)
        return -1;
    
    if((dev->rxlen==0)&&!isprint(data))
    {
        return 0;
    }
    
    if((data=='\b')&&(dev->rxlen>0))
    {
        dev->rxlen--;
         stdio_dev->put_char(data);

        return 0;
    }
    
    stdio_dev->put_char(data);
    
    dev->rxbuf[dev->rxlen++]=data;
    if(dev->rxlen==CLI_BUF_SIZE)
        dev->rxlen = 0;
    
    if((end[end_len-1]==data)&&(dev->rxlen>end_len))
    {
        msg = dev->rxbuf+(dev->rxlen-end_len);
        for(i=0;i<end_len;i++)
        {
            if(msg[i]!=end[i])
            {
                return 0;
            }     
        }
        ret = dev->rxlen-match_len;
        dev->flag&=~CLI_PROMOT;
    }
    
    
    
    return ret;
}