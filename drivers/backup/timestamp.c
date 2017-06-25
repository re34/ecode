#include "timestamp.h"


struct timestamp ftimestamp={
    .msecond = 0,
    .second = 0,
};


struct timestamp get_timestamp(void)
{
    return ftimestamp;
}

void timestamp_polling(int ms)
{
    ftimestamp.msecond += ms;
    if(ftimestamp.msecond>=1000)
    {
        ftimestamp.second ++;
        ftimestamp.msecond-=1000;
    }
}



