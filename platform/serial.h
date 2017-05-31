#ifndef __SERIAL_H__
#define __SERIAL_H__

enum{
    COM1,
    COM2,
    COM3,
    COMn
};


struct serial_dev{
    int baudrate;
    int (*get_c)(void);
    int (*put_c)(unsigned char c);
    int (*in_waiting)(void);
};



int serial_register(int fd, struct serial_dev *dev);
int serial_write(int fd, char *datas, int len);
int serial_read(int fd, char *buf, int len);
int serial_in_waiting(int fd);
#endif