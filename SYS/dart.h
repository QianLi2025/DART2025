#ifndef DART_H
#define DART_H

#include "struct_typedef.h"
#include "gpio.h"
#include "motorUsr.h"
#include "can_user.h"
#include "remote_control.h"
#include "stdlib.h"
#include "DWT.h"

#pragma packed(1)
typedef struct{
    uint8_t yawLimitPushed;//yaw限位是否触发 1轻触开关限位触发，2范围限制触发
    uint8_t pushUpperLimitPushed;//推杆上限位是否触发
    uint8_t pushLowerLimitPushed;//推杆下限位是否触发
    uint8_t pushCurrentLimitPushed;//推杆电流限位是否触发
    uint8_t magazineCurrentLimitPushed;//弹仓电流限位是否触发

    uint8_t magazineSwitch;//切换弹舱状态

    uint8_t needJiaoZhun;//校准状态 0校准完成，1需要校准
    uint8_t semiAutoState;//半自动模式状态

}dartSysState_t;
typedef struct 
{
    int16_t shootSpeed;//发射速度
    /* data */
}roket;
#pragma packed()

#define D2006CONV  (1.0f / 5000000.0f)

enum controlMode {fullAuto,semiAuto,manual,notConnected};//控制模式选择
extern enum controlMode mode;//默认手动挡
void checkControlMode();//遥控器选择控制模式
void manualTask();//手动挡任务
void semiAutoTask();//半自动档任务
void calAndSendMotor();//发送电机控制量，只在主函数循环末尾被调用
void dartSysStateCheck();
#endif
