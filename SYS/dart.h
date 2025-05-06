#ifndef DART_H
#define DART_H

#include "struct_typedef.h"
#include "gpio.h"
#include "motorUsr.h"
#include "can_user.h"
#include "remote_control.h"
#include "stdlib.h"
#include "DWT.h"
#include "minipc_protocol.h"
#include "math.h"
#include "stdio.h"
#include "string.h"
#include "usbd_cdc_if.h"
#include "usart.h"
#include "referee.h"
#include "bit_utils.h"

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
    uint8_t visualJiaoZhun;//视觉校准状态1代表已校准，0代表未校准
    uint8_t emergenceyStop;//紧急停止状态

//0比赛未开始，1比赛开始，第一发飞镖准备；2第一发飞镖发射；3第三发飞镖发射准备；4第三发飞镖发射；5第四发飞镖发射；6飞镖发射结束。
    uint8_t fullAutoState;
    uint8_t shootGoal;//打击目标 0前哨 1基地 2随机固定 3随机移动
    uint8_t ammoState;//弹夹剩余的飞镖数量，通过位操作判断，第零位代表1号飞镖，第一位为2号，以此类推 1代表已经被发射，0代表还未发射
}dartSysState_t;
typedef struct 
{
    uint16_t shootSpeed;//发射速度
    uint16_t pushSpeed;//推杆速度
    uint16_t yawPlace;//yaw位置
    uint16_t yawDelta;//miniPC修正后的yaw轴的修正量

    uint16_t shootSpeedBase;//发射速度（基地）
    uint16_t pushSpeedBase;//推杆速度（基地）
    uint16_t yawPlaceBase;//yaw位置（基地）
    uint16_t yawDeltaBase;//miniPC修正后的yaw轴的修正量（基地）
}roket;
typedef struct //对于一次发射，将其视为具有四个参数的任务。这三个参数为推杆位置、推杆速度、yaw位置和发射速度。实际建议只是用推杆置位和发射速度作为参数。不考虑推杆速度
{
    uint16_t shootPushPlace[4];
    uint16_t shootPushSpeed[4];
    uint16_t shootSpeed[4];
    uint16_t shootYawPlace[4];
    uint16_t miniPcYawDelta[4];

    uint16_t shootPushSpeedBase[4];
    uint16_t shootSpeedBase[4];
    uint16_t shootYawPlaceBase[4];
    uint16_t miniPcYawDeltaBase[4];
}roketShootingTaskT;

#pragma packed()

#define D2006CONV  (1.0f / 50000.0f)

enum controlMode {fullAuto,semiAuto,manual,notConnected};//控制模式选择
extern enum controlMode mode;//默认手动挡
void checkControlMode();//遥控器选择控制模式
void manualTask();//手动挡任务
void semiAutoTask();//半自动档任务
void calAndSendMotor();//发送电机控制量，只在主函数循环末尾被调用
void dartSysStateCheck();//对飞镖系统各类状态进行查询
void shootTaskInit(roket *r1,roket *r2,roket *r3,roket *r4);//发射任务初始化
void yawPlaceRefresh();
void pushPlaceRefreshSpeedy();
uint8_t pushYawInit();
void fullAutoTask();
void refreeDataCheck();
#endif
