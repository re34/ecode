#ifndef __TIMER_H__
#define __TIMER_H__


enum{
  TIMER1,
  TIMERn,  
};

struct timer_device{
    int (*init)(void);
    void (*start)(void);
    void (*stop)(void);
    void (*reset)(void);
    float (*read)(void);
    int (*read_ms)(void);
    int (*read_us)(void);
};

int timer_register(int fd, struct timer_device *dev);
int timer_start(int fd);
int timer_stop(int fd);
int timer_reset(int fd);
float timer_read(int fd);
int timer_read_ms(int fd);
int timer_read_us(int fd);

#endif