#include "motorUsr.h"

DJImotor_info_t D2006_motor1;
DJImotor_info_t D2006_motor2;
DJImotor_info_t D3508_motor1;
DJImotor_info_t D3508_motor2;
DJImotor_info_t D6020_motor1;

void motorInit(DJImotor_info_t *motor, uint8_t motor_id, uint16_t currentSendBaseId, uint16_t voltageSendBaseId, uint16_t feedbackBaseId)
{
    motor->motor_id = motor_id;
    motor->currentSendBaseId = currentSendBaseId;
    motor->voltageSendBaseId = voltageSendBaseId;
    motor->feedbackBaseId = feedbackBaseId;
    
    motor->set_voltage = 0;
    motor->rotor_speed = 0;
    motor->target_speed = 0;
    motor->rotor_angle = 0;
    motor->last_angle = 0;
    motor->angle_difl = 0;
    motor->absolute_angle = 0;
    motor->target_pos = 0;
    motor->torque_current = 0;
    motor->temp = 0;

    // pid_init(&motor->motor_speed_pid, (float[3]){40.0f, 3.0f, 0.0f}, 10000.0f, 8191.0f);
    // pid_init(&motor->motor_pos_pid, (float[3]){10.0f, 1.5f, 0.5f}, 10000.0f, 8191.0f);

}

