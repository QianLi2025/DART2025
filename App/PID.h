#ifndef PID_H
#define PID_H
#include "struct_typedef.h"
typedef struct _pid_struct_t
{
  float kp;
  float ki;
  float kd;
  float i_max;
  float out_max;
  
  float ref;      // target value
  float fdb;      // feedback value  
  float err[2];   // error and last error

  float p_out;
  float i_out;
  float d_out;
  float output;
}pid_struct_t;


void pid_init(pid_struct_t *pid,
              fp32 PID[3],
              float i_max,
              float out_max);
float pid_calc(pid_struct_t *pid, float ref, float fdb);//ref是目标值,fdb是电机解码的速度返回值
float pos_pid_calc(pid_struct_t *pid, float ref, float fdb);//ref是目标值,fdb是电机解码的速度返回值
void pidReset(pid_struct_t *pid);//清空积分输出


#endif
