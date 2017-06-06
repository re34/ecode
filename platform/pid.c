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
    if(pid==NULL)
        return -1;
    
    
    
    
    return 0;    
}

