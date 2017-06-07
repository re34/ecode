#ifndef __PID_H__
#define __PID_H__


struct pid{
    float set_val;
    float real_val;
    float output_val;
    float err;//偏差
    float err_last;
    float err_last_last;
    float kp,ki,kd;
    float max;
    float min;
    void *adjust_callback;
    void *monitor_callback;
};

typedef int (*pid_adjust_callback_t)(float output_val);
typedef float (*pid_monitor_callback_t)(void);

int pid_init(struct pid *pid);
int pid_set_coeff(struct pid *pid, float kp, float ki, float kd);
int pid_set_desired(struct pid *pid, float desired);
int pid_set_real_val(struct pid*pid, float real);
int pid_process(struct pid *pid);
#endif