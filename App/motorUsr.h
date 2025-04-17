#ifndef MOTORUSR_H
#define MOTORUSR_H

#include "struct_typedef.h"
#include "PID.h"

#pragma PACKED(1)
typedef struct
{
	//ID号
  uint8_t motor_id;//一位数ID
  uint16_t currentSendBaseId; //电流控制发送ID
  uint16_t voltageSendBaseId; //电压控制发送ID
  uint16_t feedbackBaseId; //接收ID
	//电流相关
  int16_t  set_voltage;		//发送信息
	//速度相关
  int16_t  rotor_speed;		//现在的转速
	int16_t  target_speed;  //目标速度
	//角度相关
  float rotor_angle;	  //现在的角度
	float last_angle;		  //上次的角度
	float angle_difl;     //角度差，用于累加（也可以用于长度计算）
	float absolute_angle; //绝对角度
	//位置相关
	int16_t target_pos;
  int16_t torque_current;		//实际转矩电流
  uint8_t temp;		//电机温度
  //PID
  pid_struct_t motor_speed_pid;
  pid_struct_t motor_pos_pid;
} DJImotor_info_t;
#pragma PACKED()

extern DJImotor_info_t D2006_motor1;
extern DJImotor_info_t D2006_motor2;
extern DJImotor_info_t D3508_motor1;
extern DJImotor_info_t D3508_motor2;
extern DJImotor_info_t D6020_motor1;

#define D6020_MOTOR1_ID 4
#define D2006_MOTOR1_ID 3
#define D2006_MOTOR2_ID 4
#define D3508_MOTOR1_ID 1
#define D3508_MOTOR2_ID 2

void motorInit(DJImotor_info_t *motor, uint8_t motor_id, uint16_t currentSendBaseId, uint16_t voltageSendBaseId, uint16_t feedbackBaseId);

#endif
