#include "pid.h"
#include "includes.h"

int pid_init(struct pid *pid)
{
    if(pid==NULL)
        return -1;
    
    pid->set_val = 0;
    pid->err = 0;
    pid->err_last = 0;
    pid->err_last_last = 0;
    pid->kp = 1.0f;
    pid->ki = 0;
    pid->kd = 0;
    
    return 0;
}


int pid_increment_calc(struct pid *pid, float feadback)
{
    float derr;
    ASSERT_PARAM(pid!=NULL);
    
    pid->err = pid->set_val - feadback;
    derr = pid->kp*(pid->err-pid->err_last)+pid->ki*pid->err\
        +pid->kd*(pid->err-2*pid->err_last+pid->err_last_last);

    pid->err_last_last = pid->err_last;
    pid->err_last = pid->err;
    
    if(pid->output_val>pid->max)
        pid->output_val = pid->max;
    
    if(pid->output_val<pid->min)
        pid->output_val = pid->min;
    
    return derr;
}

