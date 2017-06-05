#ifndef __PRINT_LOG_H__
#define __PRINT_LOG_H__
#include <stdio.h>


#define  LOG_LEVEL_CLOSE      5
#define  LOG_LEVEL_ERROR      4
#define  LOG_LEVEL_WARN       3
#define  LOG_LEVEL_INFO       2
#define  LOG_LEVEL_DEBUG      1

struct print_log_interface{
    //int getchar(void);
    int (*put_char)(unsigned char data);
    int (*get_char)(void);
};


int print_level(int level, const char *fmt, ...);
void print_log_register_io(struct print_log_interface fio);
int print_hex(const char *data, int len, char delim);

#define LOG_LEVEL       LOG_LEVEL_DEBUG

#define print_log(fmt, ...)    printf(fmt, ##__VA_ARGS__)
#define LogNewLine()          printf(NEW_LINE)

#if (LOG_LEVEL==LOG_LEVEL_CLOSE)
#define LOG_INFO(fmt,...)
#define LOG_DEBUG(fmt,...)
#define LOG_WARM(fmt,...)
#define LOG_ERROR(fmt,...)
#else
#define LOG_INFO(fmt,...)  print_level(LOG_LEVEL_INFO,fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt,...)  print_level(LOG_LEVEL_DEBUG,fmt, ##__VA_ARGS__)
#define LOG_WARM(fmt,...)  print_level(LOG_LEVEL_WARN,fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt,...)  print_level(LOG_LEVEL_ERROR,fmt, ##__VA_ARGS__)
#endif

#endif