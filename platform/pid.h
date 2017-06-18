#ifndef __PID_H__
#define __PID_H__


struct pid{
    float set_val;
    float err;//偏差
    float err_last;
    float err_last_last;
    float err_sum;
    float kp,ki,kd;
    float output;
    float output_max;
    float output_min;
};


int pid_init(struct pid *pid);
void pid_set_desired(struct pid *pid, float val);
void pid_set_threshold(struct pid *pid, float max, float min);
void pid_set_param(struct pid *pid, float kp, float ki, float kd);
float pid_position_calc(struct pid *pid, float feadback);
float pid_increment_calc(struct pid *pid, float feedback);
#endif