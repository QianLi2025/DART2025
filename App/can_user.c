#include "can_user.h"
#include "main.h"
#include "motor.h"
#include "motorUsr.h"
extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

HAL_StatusTypeDef flag111;

//3508标识符 0x200 0x1FF   反馈：0x200+ID
//2006标识符 0x200 0x1FF   反馈：0x200+ID
//6020标识符 0x1FF 0x2FF   反馈：0x204+ID
#define MOTOR_MAX_NUM 8
#define FEEDBACK_ID_BASE         0x201
#define FEEDBACK_ID_BASE_6020    0x205
#define CAN_CONTROL_ID_BASE      0x200
#define CAN_CONTROL_ID_EXTEND    0x1ff

CAN_RxHeaderTypeDef can1_rx_header;
uint8_t can1_rx_data[8];
CAN_RxHeaderTypeDef can2_rx_header;
uint8_t can2_rx_data[8];

		
static CAN_TxHeaderTypeDef  motor_tx_message;
static uint8_t              motor_can_send_data[8];

//motor data read
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    if (hcan->Instance == CAN1)
    {
        HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &can1_rx_header, can1_rx_data);
        switch (can1_rx_header.StdId)
        {
        case D2006_MOTOR1_ID+0x200:
            D2006_motor1.rotor_angle = ((can1_rx_data[0] << 8) | can1_rx_data[1]);
            D2006_motor1.rotor_speed = ((can1_rx_data[2] << 8) | can1_rx_data[3]);
            D2006_motor1.torque_current = ((can1_rx_data[4] << 8) | can1_rx_data[5]);
            D2006_motor1.temp = can1_rx_data[6];
            break;
        case D2006_MOTOR2_ID+0x200:
            D2006_motor2.rotor_angle = ((can1_rx_data[0] << 8) | can1_rx_data[1]);
            D2006_motor2.rotor_speed = ((can1_rx_data[2] << 8) | can1_rx_data[3]);
            D2006_motor2.torque_current = ((can1_rx_data[4] << 8) | can1_rx_data[5]);
            D2006_motor2.temp = can1_rx_data[6];
            break;
        case D3508_MOTOR1_ID+0x200:
            D3508_motor1.rotor_angle = ((can1_rx_data[0] << 8) | can1_rx_data[1]);
            D3508_motor1.rotor_speed = ((can1_rx_data[2] << 8) | can1_rx_data[3]);
            D3508_motor1.torque_current = ((can1_rx_data[4] << 8) | can1_rx_data[5]);
            D3508_motor1.temp = can1_rx_data[6];
            break;
        case D3508_MOTOR2_ID+0x200: 
            D3508_motor2.rotor_angle = ((can1_rx_data[0] << 8) | can1_rx_data[1]);
            D3508_motor2.rotor_speed = ((can1_rx_data[2] << 8) | can1_rx_data[3]);
            D3508_motor2.torque_current = ((can1_rx_data[4] << 8) | can1_rx_data[5]);
            D3508_motor2.temp = can1_rx_data[6];
            break;
        }
	}
    if (hcan->Instance == CAN2)
    {
       HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &can2_rx_header, can2_rx_data);
       switch (can2_rx_header.StdId)
       {
       case D6020_MOTOR1_ID+0x204:
            D6020_motor1.rotor_angle = ((can2_rx_data[0] << 8) | can2_rx_data[1]);
            D6020_motor1.rotor_speed = ((can2_rx_data[2] << 8) | can2_rx_data[3]);
            D6020_motor1.torque_current = ((can2_rx_data[4] << 8) | can2_rx_data[5]);
            D6020_motor1.temp = can2_rx_data[6];
        break;
       default:
        break;
       }
	}
}


void can1_cmd_motor(uint8_t id_range,int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4)
{

    uint32_t send_mail_box;
    motor_tx_message.StdId = (id_range == 0)?(0x200):(0x1ff);
    motor_tx_message.IDE = CAN_ID_STD;
    motor_tx_message.RTR = CAN_RTR_DATA;  
    motor_tx_message.DLC = 0x08;  
    motor_can_send_data[0] = ( motor1 >> 8);
    motor_can_send_data[1] =  motor1;
    motor_can_send_data[2] = ( motor2 >> 8);
    motor_can_send_data[3] =  motor2;
    motor_can_send_data[4] = ( motor3 >> 8);  
    motor_can_send_data[5] = motor3;
    motor_can_send_data[6] = (motor4 >> 8);
    motor_can_send_data[7] = motor4;
    HAL_CAN_AddTxMessage(&hcan1, &motor_tx_message, motor_can_send_data, &send_mail_box);
}

void can2_cmd_motor(uint8_t id_range,int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4)
{

    uint32_t send_mail_box;
    motor_tx_message.StdId = (id_range == 0)?(0x200):(0x1ff);
    motor_tx_message.IDE = CAN_ID_STD;
    motor_tx_message.RTR = CAN_RTR_DATA;  
    motor_tx_message.DLC = 0x08;  
    motor_can_send_data[0] = ( motor1 >> 8);
    motor_can_send_data[1] =  motor1;
    motor_can_send_data[2] = ( motor2 >> 8);
    motor_can_send_data[3] =  motor2;
    motor_can_send_data[4] = ( motor3 >> 8);  
    motor_can_send_data[5] = motor3;
    motor_can_send_data[6] = (motor4 >> 8);
    motor_can_send_data[7] = motor4;
    HAL_CAN_AddTxMessage(&hcan2, &motor_tx_message, motor_can_send_data, &send_mail_box);
}




