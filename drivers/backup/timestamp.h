#ifndef __TIMESTAMP_H__
#define __TIMESTAMP_H__


struct timestamp{
    int msecond;
    int second;
};


struct timestamp get_timestamp(void);
void timestamp_polling(int ms);

#endif