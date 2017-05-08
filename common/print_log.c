#include "print_log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

typedef struct{
    int level;
    char *strInfo;
}LogStrItem_t;


#define STR_LOG_CLOSE  "CLOSE"
#define STR_LOG_DEBUG   "DEBUG"
#define STR_LOG_WARN    "WARN"
#define STR_LOG_ERROR   "ERROR"
#define STR_LOG_INFO    "INFO"


const LogStrItem_t tLogStrMap[]={
    {LOG_LEVEL_CLOSE, STR_LOG_CLOSE},
    {LOG_LEVEL_DEBUG, STR_LOG_DEBUG},
    {LOG_LEVEL_WARN, STR_LOG_WARN},
    {LOG_LEVEL_ERROR, STR_LOG_ERROR},
    {LOG_LEVEL_INFO, STR_LOG_INFO},
};


static struct print_log_interface fprint_log;

static inline int print_log_putchar(int data);

void print_log_register_io(struct print_log_interface fio)
{
    fprint_log = fio;
    LOG_DEBUG("print log inited!\r\n");
}


static inline int print_log_putchar(int data)
{
    if(fprint_log.put_char==NULL)
        return -1;
    return fprint_log.put_char(data);
}

static inline int print_log_getchar(void)
{
    if(fprint_log.get_char==NULL)
        return -1;
    return fprint_log.get_char();
}

static char *LogGetLevelString(int level)
{
    int i;
    
    
    for(i=0;i<sizeof(tLogStrMap)/sizeof(tLogStrMap[0]);i++)
    {
        if(tLogStrMap[i].level==level)
        {
            return tLogStrMap[i].strInfo;
        }
    }
    return STR_LOG_INFO;
}

int print_level(int level, const char *fmt, ...)
{
    int ret;
    if(level==LOG_LEVEL_CLOSE)
        return 0;
    
    if(level>=LOG_LEVEL)
    {
        ret = printf("[");
        fflush(stdout);
        ret+=printf(LogGetLevelString(level));
        fflush(stdout);
        ret+=printf("]");
        fflush(stdout);
        
        va_list args;
        va_start(args, fmt);
        
        ret += vprintf(fmt, args);
        
        va_end(args);
        
        fflush(stdout);
        
        printf("\r\n");
        
    }
    
    return ret;
}

PUTCHAR_PROTOTYPE
{
    print_log_putchar(ch);
    return ch;
}

//#ifdef __GNU__

int __io_getchar(void)
{
    return print_log_getchar();
}

//#endif
