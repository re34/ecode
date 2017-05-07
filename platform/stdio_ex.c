#include "stdio_ex.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>


int stdio_getc(struct stdio_device *dev)
{
    return dev->getchar();
}

int stdio_putc(struct stdio_device *dev,char data)
{
    dev->putchar(data);
    
    return 0;
}

int stdio_puts(struct stdio_device *dev, const char *str)
{
    int ret;
    int len = strlen(str);
    char *pstr = (char *)str;
    
    if(dev->putchar==NULL)
        return -1;
    
    ret = len;
    while(len--)
    {
        dev->putchar(*pstr++);
    }
    
    return ret;
}

int stdio_print(struct stdio_device *dev, const char *fmt, ...)
{
    int ret;
    va_list args;
    
    if(dev->putchar==NULL)
        return -1;
    
    va_start(args, fmt);
    
    ret = vsprintf(dev->buf, fmt, args);
    
    va_end(args);
    
    dev->buf[ret]='\0';
    
    ret = stdio_puts(dev, dev->buf);
    
    return ret;
}






