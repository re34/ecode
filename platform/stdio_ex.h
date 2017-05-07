#ifndef __STDIO_EX_H__
#define __STDIO_EX_H__

#define STDIO_BUFFER_SIZE       128

#define STDIO_EOF       (-1)
#define NEW_LINE        "\r\n"

struct stdio_operations{
    int (*putchar)(unsigned char data);
    int (*getchar)(void);
};

struct stdio_device{
    char *name;
    char buf[STDIO_BUFFER_SIZE];
    struct stdio_operations;
};


int stdio_getc(struct stdio_device *dev);
int stdio_putc(struct stdio_device *dev,char data);
int stdio_puts(struct stdio_device *dev, const char *str);
int stdio_print(struct stdio_device *dev, const char *fmt, ...);

#endif