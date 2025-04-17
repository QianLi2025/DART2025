#ifndef MOTORUSR_H
#define MOTORUSR_H

#include "struct_typedef.h"
#include "PID.h"

#pragma PACKED(1)
typedef struct
{
	//ID��
  uint8_t motor_id;//һλ��ID
  uint16_t currentSendBaseId; //�������Ʒ���ID
  uint16_t voltageSendBaseId; //��ѹ���Ʒ���ID
  uint16_t feedbackBaseId; //����ID
	//�������
  int16_t  set_voltage;		//������Ϣ
	//�ٶ����
  int16_t  rotor_speed;		//���ڵ�ת��
	int16_t  target_speed;  //Ŀ���ٶ�
	//�Ƕ����
  float rotor_angle;	  //���ڵĽǶ�
	float last_angle;		  //�ϴεĽǶ�
	float angle_difl;     //�ǶȲ�����ۼӣ�Ҳ�������ڳ��ȼ��㣩
	float absolute_angle; //���ԽǶ�
	//λ�����
	int16_t target_pos;
  int16_t torque_current;		//ʵ��ת�ص���
  uint8_t temp;		//����¶�
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
