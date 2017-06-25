/*
u(t)=Kp[e(k)+T/Ti∑e(j)+Td/T[e(k)−e(k−1)]]
    =Kp*e(k)+Kp*T/Ti∑e(j)+Kp*Td/T[e(k)-e(k-1)]
    =Kp*e(k)+Ki*∑e(j)+Kd*[e(k)-e(k-1)]
Ki = Kp*T/Ti
Kd = Kp*Td/T

Δu(k)=u(k)−u(k−1)
     =Kp[e(k)-e(k-1)]+Ki*e(k)+kd[e(k)-2e(k-1)+e(k-2)]
*/

#include "pid.h"
#include "assert.h"



int pid_init(struct pid *pid)
{
    ASSERT_PARAM(pid!=NULL);
    
    pid->set_val = 0;
    pid->err = 0;
    pid->err_last = 0;
    pid->err_last_last = 0;
    pid->kp = 1.0f;
    pid->ki = 0;
    pid->kd = 0;
    
    return 0;
}

void pid_set_desired(struct pid *pid, float val)
{
    ASSERT_PARAM(pid!=NULL);
    
    pid->set_val = val;
}

void pid_set_threshold(struct pid *pid, float max, float min)
{
    ASSERT_PARAM(pid!=NULL);
    
    pid->output_max = max;
    pid->output_min = min;
}

void pid_set_param(struct pid *pid, float kp, float ki, float kd)
{
    ASSERT_PARAM(pid!=NULL);
    
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
}


float pid_position_calc(struct pid *pid, float feadback)
{
    ASSERT_PARAM(pid!=NULL);
    
    pid->err = pid->set_val - feadback;
    pid->err_sum += pid->err;
    
    pid->output = pid->kp*pid->err+pid->ki*pid->err_sum+\
                    pid->kd*(pid->err-pid->err_last);

    pid->err_last = pid->err;
    
    if(pid->output>pid->output_max)
        pid->output = pid->output_max;
    if(pid->output<pid->output_min)
        pid->output = pid->output_min;
    
    return pid->output;
}

float pid_increment_calc(struct pid *pid, float feedback)
{
    float duk = 0;
    ASSERT_PARAM(pid!=NULL);
    
    pid->err = pid->set_val-feedback;
    pid->err_sum += pid->err;
    
    duk = pid->kp*(pid->err-pid->err_last)+pid->ki*pid->err+\
          pid->kd*(pid->err-2*pid->err_last+pid->err_last_last);
          
    pid->err_last_last = pid->err_last;
    pid->err_last = pid->err;
    
    pid->output += duk;
    
    if(pid->output>pid->output_max)
        pid->output = pid->output_max;
    if(pid->output<pid->output_min)
        pid->output = pid->output_min;
    
    return pid->output;    
}




