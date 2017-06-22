#ifndef __STDIO_EX_H__
#define __STDIO_EX_H__

#define STDIO_BUFFER_SIZE       128

#define STDIO_EOF       (-1)
#define NEW_LINE        "\r\n"


struct stdioex_device{
    char *name;
    char buf[STDIO_BUFFER_SIZE];
    int (*put_char)(unsigned char data);
    int (*get_char)(void);
};


int stdio_getc(struct stdioex_device *dev);
int stdio_putc(struct stdioex_device *dev,char data);
int stdio_puts(struct stdioex_device *dev, const char *str);
int stdio_print(struct stdioex_device *dev, const char *fmt, ...);

#endif