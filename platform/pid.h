#ifndef __PID_H__
#define __PID_H__


struct pid{
    float set_val;
    float read_val;
    float err;//偏差
    float err_last;
    float err_last_last;
    float kp,ki,kd;
    float max;
    float min;
    void *adjust_callback;
    void *monitor_callback;
};

int pid_init(struct pid *pid);

#endif