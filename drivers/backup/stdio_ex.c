#include "stdio_ex.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>


int stdio_getc(struct stdioex_device *dev)
{
    return dev->get_char();
}

int stdio_putc(struct stdioex_device *dev,char data)
{
    dev->put_char(data);
    
    return 0;
}

int stdio_puts(struct stdioex_device *dev, const char *str)
{
    int ret;
    int len = strlen(str);
    char *pstr = (char *)str;
    
    if(dev->put_char==NULL)
        return -1;
    
    ret = len;
    while(len--)
    {
        dev->put_char(*pstr++);
    }
    
    return ret;
}

int stdio_print(struct stdioex_device *dev, const char *fmt, ...)
{
    int ret;
    va_list args;
    
    if(dev->put_char==NULL)
        return -1;
    
    va_start(args, fmt);
    
    ret = vsprintf(dev->buf, fmt, args);
    
    va_end(args);
    
    dev->buf[ret]='\0';
    
    ret = stdio_puts(dev, dev->buf);
    
    return ret;
}






