#ifndef __SERIAL_H__
#define __SERIAL_H__
#include "../common/includes.h"


struct serial_operations{
    int (*get_byte)(void);
    int (*write_byte)(UInt8 byte);
};

typedef struct{
    int baudrate;
    struct serial_operations ops;
}Serial_t;


int serial_init(void);

#endif