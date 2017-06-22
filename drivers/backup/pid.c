#include "pid.h"
#include "includes.h"

int pid_init(struct pid *pid)
{
    if(pid==NULL)
        return -1;
    
    pid->set_val = 0;
    pid->real_val = 0;
    pid->err = 0;
    pid->err_last = 0;
    pid->err_last_last = 0;
    pid->kp = 1.0f;
    pid->ki = 0;
    pid->kd = 0;
    pid->adjust_callback = 0;
    pid->monitor_callback = 0;
    
    return 0;
}

int pid_set_coeff(struct pid *pid, float kp, float ki, float kd)
{
    if(pid==NULL)
        return -1;
    
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    
    return 0;
}

int pid_set_desired(struct pid *pid, float desired)
{
    if(pid==NULL)
        return -1;
    
    pid->set_val = desired;
    
    return 0;
}

int pid_set_real_val(struct pid*pid, float real)
{
    if(pid==NULL)
        return -1;
    
    pid->real_val = real;
    
    return 0;
}

int pid_process(struct pid *pid)
{
    float err;
    if(pid==NULL)
        return -1;
    
    if(pid->monitor_callback!=NULL)
        pid->real_val = ((pid_monitor_callback_t)pid->monitor_callback)();
    else
        return -2;
    
    pid->err = pid->set_val - pid->real_val;
    err = pid->kp*(pid->err-pid->err_last)+pid->ki*pid->err\
        +pid->kd*(pid->err-2*pid->err_last+pid->err_last_last);
    pid->output_val += err;
    pid->err_last_last = pid->err_last;
    pid->err_last = pid->err;
    
    if(pid->output_val>pid->max)
        pid->output_val = pid->max;
    
    if(pid->output_val<pid->min)
        pid->output_val = pid->min;
    
    if(pid->adjust_callback!=NULL)
        ((pid_adjust_callback_t)pid->adjust_callback)(pid->output_val);
    else
        return -2;
    
    
    return 0;    
}

