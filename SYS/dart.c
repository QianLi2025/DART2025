#include "dart.h"

enum controlMode mode=manual;//默认手动挡

dartSysState_t dartState={
                        .yawLimitPushed=0,
                        .pushUpperLimitPushed=0,
                        .pushLowerLimitPushed=0,
                        .pushCurrentLimitPushed=0,
                        .magazineCurrentLimitPushed=0,
                        .magazineSwitch=0,
                        .needJiaoZhun=1,
                        .semiAutoState=1,
                        .visualJiaoZhun=0,
                        .emergenceyStop=0,
                        .fullAutoState=0,
                        .shootGoal=0,
                        .ammoState=0};//系统状态
roketShootingTaskT roketShootTask={
                        .shootPushPlace={0,0,0,0},
                        .shootPushSpeed={0,0,0,0},
                        .shootSpeed={0,0,0,0},
                        .shootYawPlace={0,0,0,0},
                        .miniPcYawDelta={0,0,0,0}};//发射任务                      
void shootTaskInit(roket *r1,roket *r2,roket *r3,roket *r4){
    roketShootTask.shootSpeed[0]=r1->shootSpeed;
    roketShootTask.shootSpeed[1]=r2->shootSpeed;
    roketShootTask.shootSpeed[2]=r3->shootSpeed;
    roketShootTask.shootSpeed[3]=r4->shootSpeed;
    roketShootTask.shootPushPlace[0]=4200;
    roketShootTask.shootPushPlace[1]=5700;
    roketShootTask.shootPushPlace[2]=4200;
    roketShootTask.shootPushPlace[3]=5700;
    roketShootTask.shootYawPlace[0]=r1->yawPlace;
    roketShootTask.shootYawPlace[1]=r2->yawPlace;
    roketShootTask.shootYawPlace[2]=r3->yawPlace;
    roketShootTask.shootYawPlace[3]=r4->yawPlace;
    roketShootTask.miniPcYawDelta[0]=r1->yawDelta;
    roketShootTask.miniPcYawDelta[1]=r2->yawDelta;
    roketShootTask.miniPcYawDelta[2]=r3->yawDelta;
    roketShootTask.miniPcYawDelta[3]=r4->yawDelta;

    roketShootTask.shootSpeedBase[0]=r1->shootSpeedBase;
    roketShootTask.shootSpeedBase[1]=r2->shootSpeedBase;
    roketShootTask.shootSpeedBase[2]=r3->shootSpeedBase;
    roketShootTask.shootSpeedBase[3]=r4->shootSpeedBase;
    roketShootTask.shootYawPlaceBase[0]=r1->yawPlaceBase;
    roketShootTask.shootYawPlaceBase[1]=r2->yawPlaceBase;
    roketShootTask.shootYawPlaceBase[2]=r3->yawPlaceBase;
    roketShootTask.shootYawPlaceBase[3]=r4->yawPlaceBase;
    roketShootTask.miniPcYawDeltaBase[0]=r1->yawDeltaBase;
    roketShootTask.miniPcYawDeltaBase[1]=r2->yawDeltaBase;
    roketShootTask.miniPcYawDeltaBase[2]=r3->yawDeltaBase;
    roketShootTask.miniPcYawDeltaBase[3]=r4->yawDeltaBase;
}

void refreeDataCheck(){//检查裁判系统数据，修改自动挡模式飞镖数据
    //比赛开始，跳转到第一发飞镖准备状态
    if(rfData.game_status.game_progress==4&&dartState.fullAutoState==0){dartState.fullAutoState=1;}
    //比赛结算直接跳转到停止状态
    else if(rfData.game_status.game_progress==5){dartState.fullAutoState=9;}
    //其它情况认为在准备阶段
//    else{dartState.fullAutoState=0;}
    switch (dartState.cangmenState)
    {
    case 2://第一次完全开启
        switch (dartState.fullAutoState)
        {
        case 1:dartState.fullAutoState=2;break;//比赛已经开始，飞镖架处于第一组飞镖准备状态
        default:break;
        }
    break;
    case 3://第一次正在关闭
        switch (dartState.fullAutoState)//使用switch方便后续拓展
        {
            case 2:dartState.fullAutoState=5;dartState.visualJiaoZhun=0;dartState.ammoState=2;break;//飞镖架处于飞镖发射状态，跳转至下一次准备
            case 3:dartState.fullAutoState=5;dartState.visualJiaoZhun=0;dartState.ammoState=2;break;//飞镖架处于飞镖发射状态，跳转至下一次准备
            case 4:dartState.fullAutoState=5;dartState.visualJiaoZhun=0;dartState.ammoState=2;break;//飞镖架处于飞镖发射状态，跳转至下一次准备
            case 6:dartState.fullAutoState=9;break;//飞镖架处于飞镖发射状态，跳转至下一次准备
            case 7:dartState.fullAutoState=9;break;//飞镖架处于飞镖发射状态，跳转至下一次准备
            case 8:dartState.fullAutoState=9;break;//飞镖架处于飞镖发射状态，跳转至下一次准备
            default:break;
        }
    break;
    case 4://第一次完全关闭
        switch (dartState.fullAutoState)//使用switch方便后续拓展
        {
            case 2:dartState.fullAutoState=5;dartState.visualJiaoZhun=0;dartState.ammoState=2;break;//飞镖架处于飞镖发射状态，跳转至下一次准备
            case 3:dartState.fullAutoState=5;dartState.visualJiaoZhun=0;dartState.ammoState=2;break;//飞镖架处于飞镖发射状态，跳转至下一次准备
            case 4:dartState.fullAutoState=5;dartState.visualJiaoZhun=0;dartState.ammoState=2;break;//飞镖架处于飞镖发射状态，跳转至下一次准备
            case 6:dartState.fullAutoState=9;break;//飞镖架处于飞镖发射状态，跳转至下一次准备
            case 7:dartState.fullAutoState=9;break;//飞镖架处于飞镖发射状态，跳转至下一次准备
            case 8:dartState.fullAutoState=9;break;//飞镖架处于飞镖发射状态，跳转至下一次准备
            default:break;
        }
    break;
    case 6://第二次完全打开
        switch (dartState.fullAutoState)
        {
        case 1:dartState.fullAutoState=2;break;//比赛已经开始，飞镖架处于第一组飞镖准备状态
        case 5:dartState.fullAutoState=6;break;//比赛已经开始，飞镖架处于第二组飞镖准备状态
        default:break;
        }
    break;
    case 7://第二次正在关闭
        switch (dartState.fullAutoState)//使用switch方便后续拓展
        {
            case 2:dartState.fullAutoState=5;dartState.visualJiaoZhun=0;dartState.ammoState=2;break;//飞镖架处于飞镖发射状态，跳转至下一次准备
            case 3:dartState.fullAutoState=5;dartState.visualJiaoZhun=0;dartState.ammoState=2;break;//飞镖架处于飞镖发射状态，跳转至下一次准备
            case 4:dartState.fullAutoState=5;dartState.visualJiaoZhun=0;dartState.ammoState=2;break;//飞镖架处于飞镖发射状态，跳转至下一次准备
            case 6:dartState.fullAutoState=9;break;//飞镖架处于飞镖发射状态，跳转至下一次准备
            case 7:dartState.fullAutoState=9;break;//飞镖架处于飞镖发射状态，跳转至下一次准备
            case 8:dartState.fullAutoState=9;break;//飞镖架处于飞镖发射状态，跳转至下一次准备
            default:break;
        }
    break;
    case 8://第二次完全关闭
        switch (dartState.fullAutoState)//使用switch方便后续拓展
        {
            case 2:dartState.fullAutoState=5;dartState.visualJiaoZhun=0;dartState.ammoState=2;break;//飞镖架处于飞镖发射状态，跳转至下一次准备
            case 3:dartState.fullAutoState=5;dartState.visualJiaoZhun=0;dartState.ammoState=2;break;//飞镖架处于飞镖发射状态，跳转至下一次准备
            case 4:dartState.fullAutoState=5;dartState.visualJiaoZhun=0;dartState.ammoState=2;break;//飞镖架处于飞镖发射状态，跳转至下一次准备
            case 6:dartState.fullAutoState=9;break;//飞镖架处于飞镖发射状态，跳转至下一次准备
            case 7:dartState.fullAutoState=9;break;//飞镖架处于飞镖发射状态，跳转至下一次准备
            case 8:dartState.fullAutoState=9;break;//飞镖架处于飞镖发射状态，跳转至下一次准备
            default:break;
        }
    break;
    default:
        break;
    }

    
    
}
void remoteControllerDataCheck(){
        //使用遥控器传递信息的逻辑，其优先级比裁判系统传递优先级较高
    if(rc_ctrl.rc.ch[1]>500&&dartState.fullAutoState==0){dartState.fullAutoState=1;}//模拟比赛开始逻辑
    if(rc_ctrl.rc.ch[3]>500){//遥控模拟裁判系统数据，模拟裁判系统飞镖舱门完全打开逻辑
        
        if(dartState.fullAutoState==1){dartState.fullAutoState=2;}
        else if(dartState.fullAutoState==5){dartState.fullAutoState=6;}
    }
    if(rc_ctrl.rc.ch[3]<-500){//遥控模拟裁判系统数据,模拟裁判系统结束逻辑
        if(dartState.fullAutoState<5){dartState.fullAutoState=5;dartState.ammoState=2;}
        else if(dartState.fullAutoState<9&&dartState.fullAutoState>5){dartState.fullAutoState=9;}
    }
}

void checkCangmenState(){//检测舱门的状态，必须放在checkControlMode前面执行
    switch (dartState.cangmenState)
    {
    case 0://1舱门正在第一次打开，0舱门没有动作
        if(rfData.game_status.game_progress==4&&rfData.dart_client_cmd.dart_launch_opening_status==2){
            dartState.cangmenState=1;
        }
    break;
    case 1://2舱门第一次完全打开
        if(rfData.game_status.game_progress==4&&rfData.dart_client_cmd.dart_launch_opening_status==0){
            dartState.cangmenState=2;
        }
    break;
    case 2://3舱门第一次正在关闭
        if(rfData.game_status.game_progress==4&&rfData.dart_client_cmd.dart_launch_opening_status==2){
            dartState.cangmenState=3;
        }
    break;
    case 3://4舱门第一次完全关闭
        if(rfData.game_status.game_progress==4&&rfData.dart_client_cmd.dart_launch_opening_status==1){
            dartState.cangmenState=4;
        }
    break;
    case 4://5舱门第二次正在开启
        if(rfData.game_status.game_progress==4&&rfData.dart_client_cmd.dart_launch_opening_status==2){
            dartState.cangmenState=5;
        }
    break;
    case 5://6舱门第二次完全开启
        if(rfData.game_status.game_progress==4&&rfData.dart_client_cmd.dart_launch_opening_status==0){
            dartState.cangmenState=6;
        }
    break;
    case 6://7舱门第二次正在关闭
        if(rfData.game_status.game_progress==4&&rfData.dart_client_cmd.dart_launch_opening_status==2){
            dartState.cangmenState=7;
        }
    break;
    case 7://8舱门第二次完全关闭
        if(rfData.game_status.game_progress==4&&rfData.dart_client_cmd.dart_launch_opening_status==1){
            dartState.cangmenState=8;
        }
    break;
    default:
    break;
    }
    
}

void checkControlMode(){
    switch (rc_ctrl.rc.s[1])//遥控器选择控制模式
    {
    case 1:mode=fullAuto;//全自动模式fuli auto shooter
        switch (rc_ctrl.rc.s[0])
        {//裁判系统测试ok
            case 1:
                dartState.shootGoal=get_bits_6_7_u16(rfData.dart_info.dart_info);
                switch (dartState.shootGoal)//目前来讲，随机固定和随即移动靶的目标都选为基地
                {
                case 2:dartState.shootGoal=1;break;
                case 3:dartState.shootGoal=1;break;
                default:break;
                }
                refreeDataCheck();
                remoteControllerDataCheck();
            break;//下，采用裁判系统的回传数据作为发射目标的选择依据
            case 3:
                dartState.shootGoal=0;
                remoteControllerDataCheck();
            break;//中，瞄准前哨
            case 2:
                dartState.shootGoal=1;
                remoteControllerDataCheck();
            break;//上，瞄准基地固定靶
            default:break;
        }

    break;
    case 2:mode=semiAuto;//半自动模式
        if(dartState.semiAutoState!=0){//紧急停止
            if(rc_ctrl.rc.ch[1]<-500&&dartState.semiAutoState!=7){dartState.emergenceyStop=dartState.semiAutoState;dartState.semiAutoState=7;}//进入紧急停止状态
            else if(rc_ctrl.rc.ch[1]>500&&dartState.emergenceyStop!=0){dartState.semiAutoState=dartState.emergenceyStop;dartState.emergenceyStop=0;}
        }
    break;
    case 3:mode=manual;break;//手动模式
    default:mode=notConnected;break;
    }


}
//向minipc发送yaw，时间，第几发，是否使用视觉校准的调试信息
void minipcDebugDataSend(const float * yaw,const float * t,const uint8_t * ammo,const uint8_t * isVisual){
    minipc.mcu2minipc.yaw=*yaw;
    minipc.mcu2minipc.time=*t;
    minipc.mcu2minipc.ammo=*ammo;
    minipc.mcu2minipc.isVisual=*isVisual;
    minipc_upgrade(&minipc);
    minipc.mcu2minipc_buf[sizeof(minipc.mcu2minipc)]=0xaa;
    CDC_Transmit_FS(minipc.mcu2minipc_buf,sizeof(minipc.mcu2minipc)+1);
}

void limitProtection(){
    switch (dartState.yawLimitPushed)//对yaw限位进行保护
    {
    case 1:
    if(D6020_motor1.set_voltage>0||D6020_motor1.target_speed>0){
        D6020_motor1.set_voltage=0;D6020_motor1.target_speed=0;
        pidReset(&D6020_motor1.motor_speed_pid);
        D6020_motor1.set_voltage=pid_calc(&D6020_motor1.motor_speed_pid,D6020_motor1.target_speed,D6020_motor1.rotor_speed);
    }  
        break;
    case 2://yaw位置限位
    if(D6020_motor1.set_voltage<0||D6020_motor1.target_speed<0){
        D6020_motor1.target_speed=0;
        pidReset(&D6020_motor1.motor_speed_pid);
        D6020_motor1.set_voltage=pid_calc(&D6020_motor1.motor_speed_pid,D6020_motor1.target_speed,D6020_motor1.rotor_speed);
    }  
        break;
    default:
        break;
    }
    if(dartState.pushUpperLimitPushed){//上限位
        if(D2006_motor1.set_voltage<0||D2006_motor1.target_speed<0){
            D2006_motor1.set_voltage=0;D2006_motor1.target_speed=0;
            D2006_motor1.set_voltage=pid_calc(&D2006_motor1.motor_speed_pid, D2006_motor1.target_speed, D2006_motor1.rotor_speed);
        }   
    }
    if(dartState.pushLowerLimitPushed){//下限位
        if(D2006_motor1.set_voltage>0||D2006_motor1.target_speed>0){
            D2006_motor1.set_voltage=0;D2006_motor1.target_speed=0;
            D2006_motor1.set_voltage=pid_calc(&D2006_motor1.motor_speed_pid, D2006_motor1.target_speed, D2006_motor1.rotor_speed);
        }   
    }
    // if(dartState.pushCurrentLimitPushed>99){//电流限位
    //     D2006_motor1.set_voltage=0;D2006_motor1.target_speed=0;
    //     D2006_motor1.set_voltage=pid_calc(&D2006_motor1.motor_speed_pid, D2006_motor1.target_speed, D2006_motor1.rotor_speed);
    // }
    switch (dartState.pushCurrentLimitPushed)
    {
    case 100:
        D2006_motor1.set_voltage=0;D2006_motor1.target_speed=100;
        D2006_motor1.set_voltage=pid_calc(&D2006_motor1.motor_speed_pid, D2006_motor1.target_speed, D2006_motor1.rotor_speed);
    break;
    case -100:
        D2006_motor1.set_voltage=0;D2006_motor1.target_speed=-100;
        D2006_motor1.set_voltage=pid_calc(&D2006_motor1.motor_speed_pid, D2006_motor1.target_speed, D2006_motor1.rotor_speed);
        break;
    default:
        break;
    }
}
void calAndSendMotor(){//计算和发送电机控制量
    D2006_motor1.set_voltage=pid_calc(&D2006_motor1.motor_speed_pid, D2006_motor1.target_speed, D2006_motor1.rotor_speed);
    D2006_motor2.set_voltage=pid_calc(&D2006_motor2.motor_speed_pid, D2006_motor2.target_speed, D2006_motor2.rotor_speed);
    D3508_motor1.set_voltage=pid_calc(&D3508_motor1.motor_speed_pid, D3508_motor1.target_speed, D3508_motor1.rotor_speed);
    D3508_motor2.set_voltage=pid_calc(&D3508_motor2.motor_speed_pid, D3508_motor2.target_speed, D3508_motor2.rotor_speed);
    //6020电机可能进行位置环计算，于是进行逻辑判断
    switch (D6020_motor1.target_pos)
    {
        case -255:break;
        case -244:D6020_motor1.target_speed=pid_calc(&D6020_motor1.motor_pos_pixel_pid,D6020_motor1.target_pixel,D6020_motor1.nowPixel);break;
        default:  D6020_motor1.target_speed=-pid_calc(&D6020_motor1.motor_pos_pid,D6020_motor1.target_pos,D6020_motor1.absolute_angle);break;
    }
    D6020_motor1.set_voltage=pid_calc(&D6020_motor1.motor_speed_pid, D6020_motor1.target_speed, D6020_motor1.rotor_speed);
        
    limitProtection();//各个电机保护，这个函数若修改，可以减少计算量
    can2_cmd_motor(1,0,0,0,D6020_motor1.set_voltage);
    can1_cmd_motor(0,D3508_motor1.set_voltage,D3508_motor2.set_voltage,D2006_motor1.set_voltage,D2006_motor2.set_voltage);
}

uint8_t pushUpperLimitCheck(){//上限位检测
    return HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_9);
}
uint8_t pushLowerLimitCheck(){//下限位检测
    return HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_8);
}
uint8_t yawLimitCheck(){//yaw限位检测
    return HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
    if(huart->Instance==UART5)
	{
		//把收到的一包数据通过串口回传
		//HAL_UART_Transmit(&huart5,rfDataBuf,Size,0xff);
        referee_fbkdata(&rfData,rfDataBuf,150);
		//再次开启空闲中断接收，不然只会接收一次数据
		HAL_UARTEx_ReceiveToIdle_IT(&huart5,rfDataBuf,150);
	}

}

//对飞镖系统各类状态进行查询
void dartSysStateCheck(void)
{
    // 1. 首次查询限位开关，缓存 yawLimit
    uint8_t yawPushed = yawLimitCheck();
    dartState.pushUpperLimitPushed = pushUpperLimitCheck();
    dartState.pushLowerLimitPushed = pushLowerLimitCheck();
    // 2. 推杆电机电流超限检测（|t|>5000）
    int16_t tc1 = D2006_motor1.torque_current;
    // dartState.pushCurrentLimitPushed = (tc1 > 4000) ? 1 : ((tc1 < -4000) ? 2 : 0);
    if(tc1>1000){
        if(dartState.pushCurrentLimitPushed<100){
            dartState.pushCurrentLimitPushed++;
        }
    }
    else if(tc1<-1000){
        if(dartState.pushCurrentLimitPushed>-100){
            dartState.pushCurrentLimitPushed--;
        }
    }
    else{
        dartState.pushCurrentLimitPushed=0;
    }
    // 3. 弹夹电机电流超限检测
    int16_t tc2 = D2006_motor2.torque_current;
    dartState.magazineCurrentLimitPushed = (tc2 > 5000) || (tc2 < -5000);
    // 4. Yaw 位置超限：如果角度>3000，强制设为 2
    int16_t yawAng = D6020_motor1.absolute_angle;
    if (yawAng > 3260) {yawPushed = 2;}
    dartState.yawLimitPushed = yawPushed;
    // 5. 只要 yawLimitPushed 为1（触发轻触开关），就清零绝对角度
    if (yawPushed==1) {D6020_motor1.absolute_angle = 0;}
    // 6. 如果下限位触发，则清零推杆电机角度
    if (dartState.pushLowerLimitPushed) {D2006_motor1.absolute_angle = 0;}
}

uint8_t speedCheck(int16_t speed2check,int16_t targetSpeed,uint16_t range){
return (abs(speed2check-targetSpeed)<=range)? 1:0 ;
}

//对各个电机进行速度设置、停止等操作
void yawStop(){D6020_motor1.target_speed=0;D6020_motor1.target_pos=-255;pidReset(&D6020_motor1.motor_speed_pid);}//yaw电机停止
void pushStop(){ D2006_motor1.target_speed=0;D2006_motor1.target_pos=-255;}//推杆电机停止
void magazineStop(){ D2006_motor2.target_speed=0;}//弹夹电机停止
void shootingCircleStop(){D3508_motor1.target_speed=0;D3508_motor2.target_speed=0;} //摩擦轮停止
void yawSetSpeed(int16_t speed){D6020_motor1.target_speed=-speed;D6020_motor1.target_pos=-255;}
void pushSetSpeed(int16_t speed){D2006_motor1.target_speed=-speed;D2006_motor1.target_pos=-255;}
void magazineSetSpeed(int16_t speed){D2006_motor2.target_speed=speed;}
void shootingCircleSetSpeed(int16_t speed){D3508_motor1.target_speed=-speed;D3508_motor2.target_speed=speed;}

uint8_t shootingCircleSetSpeedWithSpeedCheck(int16_t speed){
    D3508_motor1.target_speed=-speed;D3508_motor2.target_speed=speed;
    return speedCheck(D3508_motor1.rotor_speed,D3508_motor1.target_speed,30)&&speedCheck(D3508_motor2.rotor_speed,D3508_motor2.target_speed,30);
}

//对推杆电机和yaw电机复位
uint8_t pushYawInit(){
   dartSysStateCheck();
   if(dartState.pushLowerLimitPushed!=1){pushSetSpeed(-30000);}else{pushStop();}
   if(dartState.yawLimitPushed!=1){yawSetSpeed(-8000);}else{yawStop();}
   if(dartState.pushLowerLimitPushed && dartState.yawLimitPushed){
       pushStop();yawStop();
       D6020_motor1.last_angle=D6020_motor1.rotor_angle;//为计算绝对距离做准备
       D2006_motor1.last_angle=DWTGetTimeMs();//为计算绝对距离做准备
       dartState.needJiaoZhun=0;
       return 1;
   }
   return 0;
}

// 对yaw轴电机（D6020motor1）的绝对位置进行更新，使用6020转子角度对绝对位子进行计算
void yawPlaceRefresh(){
    D6020_motor1.angle_difl=D6020_motor1.rotor_angle-D6020_motor1.last_angle;
    D6020_motor1.last_angle=D6020_motor1.rotor_angle;
    if(D6020_motor1.angle_difl>8191/2){D6020_motor1.angle_difl-=8191;}else if(D6020_motor1.angle_difl<-8191/2){D6020_motor1.angle_difl+=8191;}
    D6020_motor1.angle_difl/=100;
    D6020_motor1.absolute_angle-=D6020_motor1.angle_difl;
}

//使用速度对推杆位置进行计算
void pushPlaceRefreshSpeedy(){
    float nowMS=DWTGetTimeMs();
    DJImotor_info_t *m = &D2006_motor1;
    m->angle_difl=nowMS-m->last_angle;
    m->last_angle=nowMS;
    m->absolute_angle-=m->rotor_speed*m->angle_difl*D2006CONV;
}

//令推杆电机回到0位置
uint8_t pushGoBack(){
    if(dartState.pushLowerLimitPushed){pushStop();return 1;}
    else{pushSetSpeed(-20000);return 0;}
}

uint8_t pushGoTop(int16_t speed){
    if(dartState.pushUpperLimitPushed){pushStop();return 1;}
    else{pushSetSpeed(speed);return 0;}
}

//对推杆电机进行设置，没有使用位置环
uint8_t pushSetPlace(int16_t position){
    if(position>7000){return pushGoTop(20000);}
    if(position==0){return pushGoBack();}
    if(D2006_motor1.absolute_angle<position){pushSetSpeed(20000);return 0;}
    else{pushSetSpeed(0);return 1;}
}

//对yaw电机位置进行设置，使用位置环计算yaw电机
uint8_t yawSetPlace(int16_t position){
    D6020_motor1.target_pos=position;
    if(fabs(D6020_motor1.absolute_angle-position)<1){return 1;}
    else{return 0;}
}
//通过视觉的方式计算yaw轴的位置
uint8_t yawSetPlaceVisual(int16_t yawDelta){
    D6020_motor1.target_pos=-244;
    D6020_motor1.nowPixel=minipc.minipc2mcu.distance;
    D6020_motor1.target_pixel=-yawDelta;
    if(abs(D6020_motor1.nowPixel+yawDelta)<=2&&abs(D6020_motor1.set_voltage)<1500)//当位置达到且速度小于一定时才认为成功置位
    {
        dartState.visualJiaoZhun=1;
    }
    return dartState.visualJiaoZhun;
}
//弹夹切换函数，1代表靠右侧弹夹，2代表靠左侧弹夹
void magazineSwitch(uint8_t switchTo){
    if(switchTo==1&&dartState.magazineSwitch!=11){dartState.magazineSwitch=1;}
    else if(switchTo==2&&dartState.magazineSwitch!=21){dartState.magazineSwitch=2;}
    if(dartState.magazineCurrentLimitPushed){
        magazineStop();
        switch (dartState.magazineSwitch)
        {
        case 1:dartState.magazineSwitch=11;break;
        case 2:dartState.magazineSwitch=21;break;
        default:break;
        }
        return;
    }
    else{
        switch (dartState.magazineSwitch){
        case 1:magazineSetSpeed(1000);break;
        case 2:magazineSetSpeed(-1000);break;
        default:magazineStop();break;}
    }
}

//发射准备函数，可以将弹夹、yaw、推杆置到指定位置，待完成后跳转
uint8_t shootPrepare(uint8_t shootingMagazine,int16_t yawSet,int16_t pushSet){
    shootingCircleSetSpeed(3000);
    yawSetPlace(yawSet);//yaw轴预置位是？
    if(dartState.pushLowerLimitPushed){magazineSwitch(shootingMagazine);}//使用哪组弹夹射击？
    else{pushGoBack();}//推杆归位
    if(dartState.magazineSwitch==10*shootingMagazine+1){//若弹夹完成归位
        if(pushSetPlace(pushSet))//推杆预置位是？若推杆完成归位，则返回真
        {
            return 1;
        }
    }
    return 0;
}
//fullAuto模式专用的发射准备函数，可以调整摩擦轮准备速度
uint8_t shootPrepareFullAuto(uint8_t shootingMagazine,int16_t pushSet){
    int16_t yawSet=0;
    int16_t shootingCircleSpeed=0;
    switch (dartState.shootGoal)
    {
    case 0://前哨
    if(dartState.ammoState==0){yawSet=roketShootTask.shootYawPlace[0];shootingCircleSpeed=roketShootTask.shootSpeed[0];}
    else{yawSet=roketShootTask.shootYawPlace[2];shootingCircleSpeed=roketShootTask.shootSpeed[2];}
    break;
    case 1://基地
        if(dartState.ammoState==0){yawSet=roketShootTask.shootYawPlaceBase[0];shootingCircleSpeed=roketShootTask.shootSpeedBase[0];}
        else{yawSet=roketShootTask.shootYawPlaceBase[2];shootingCircleSpeed=roketShootTask.shootSpeedBase[2];}
    break;
    default:break;
    }
    shootingCircleSetSpeed(shootingCircleSpeed);//摩擦轮准备速度为？场上需要注意在闸门开启前不能发射
    yawSetPlace(yawSet);//yaw轴预置位是？
    if(dartState.pushLowerLimitPushed){magazineSwitch(shootingMagazine);}//使用哪组弹夹射击？
    else{pushGoBack();}//推杆归位
    if(dartState.magazineSwitch==10*shootingMagazine+1){//若弹夹完成归位
        if(pushSetPlace(pushSet))//推杆预置位是？若推杆完成归位，则返回真
        {
            return 1;
        }
    }
    return 0;
}

uint8_t SCYawPushPrepare(uint8_t needPushSet){//根据发射目标和发射发数自动准备yaw、推杆、摩擦轮位置和速度
    uint8_t returnValue=1;
    uint16_t shootingCircleSpeed=0;
    uint16_t yawPlace=0;
    uint16_t pushPlace=D2006_motor1.absolute_angle;
    uint8_t ammo=dartState.ammoState;
    uint8_t magazine=0;
    uint16_t yawDelta=0;
    switch (dartState.shootGoal)
    {
    case 0:shootingCircleSpeed=roketShootTask.shootSpeed[ammo];yawPlace=roketShootTask.shootYawPlace[ammo];yawDelta=roketShootTask.miniPcYawDelta[ammo]; break;
    case 1:shootingCircleSpeed=roketShootTask.shootSpeedBase[ammo];yawPlace=roketShootTask.shootYawPlaceBase[ammo];yawDelta=roketShootTask.miniPcYawDeltaBase[ammo]; break;
    default:break;
    }
    if(ammo==0||ammo==2){pushPlace=2400;}//若为第一发和第三发，则需要调整推杆位置，其它情况因为本身处于预置位所以不用调正
    else if(ammo==1||ammo==3){pushPlace=4200;}
    if(ammo<2){magazine=1;}//第一第二发，弹夹为1
    else{magazine=2;}//第三第四发，弹夹为2
    if(dartState.magazineSwitch==10*magazine+1||ammo==1||ammo==3){if(needPushSet){returnValue=pushSetPlace(pushPlace)*returnValue;}else{pushStop();}}//若完成调整或不需要调整，则调整推杆
    else{
        if (pushGoBack()){magazineSwitch(magazine);}       
        returnValue=0;
    }//未完成
    returnValue=shootingCircleSetSpeedWithSpeedCheck(shootingCircleSpeed)*returnValue;
    // if(needYawSet){returnValue=yawSetPlace(yawPlace)*returnValue;}
    if(minipc.no_data_time>5000||minipc.minipc2mcu.distance==10000){//判定miniPC是否离线,若离线则不调整
        returnValue=yawSetPlace(yawPlace)*returnValue;//调整yaw
    }
    else if(minipc.no_data_time<100){//minipc没离线，使用minipc校准yaw轴
        returnValue=yawSetPlaceVisual(yawDelta)*returnValue;
    }
    return returnValue;
}

uint8_t SCYawPushPrepareHalf(){//根据发射目标和发射发数自动准备yaw、推杆、摩擦轮位置和速度
    uint8_t returnValue=1;
    uint16_t shootingCircleSpeed=0;
    uint16_t yawPlace=0;
    uint8_t ammo=dartState.ammoState;
    uint16_t yawDelta=0;
    switch (dartState.shootGoal)
    {
    case 0:shootingCircleSpeed=roketShootTask.shootSpeed[ammo];yawPlace=roketShootTask.shootYawPlace[ammo];yawDelta=roketShootTask.miniPcYawDelta[ammo]; break;
    case 1:shootingCircleSpeed=roketShootTask.shootSpeedBase[ammo];yawPlace=roketShootTask.shootYawPlaceBase[ammo];yawDelta=roketShootTask.miniPcYawDeltaBase[ammo]; break;
    default:break;
    }
    returnValue=shootingCircleSetSpeedWithSpeedCheck(shootingCircleSpeed)*returnValue;
    if(minipc.no_data_time>5000||minipc.minipc2mcu.distance==10000){//判定miniPC是否离线,若离线则不调整
        returnValue=yawSetPlace(yawPlace)*returnValue;//调整yaw
    }
    else if(minipc.no_data_time<500){//minipc没离线，使用minipc校准yaw轴
        returnValue=yawSetPlaceVisual(yawDelta)*returnValue;
    }
    pushStop();
    return returnValue;
}

uint8_t roketShootFull(){//发射函数，为保证发射顺畅，发射前需要保证摩擦轮速度达到目标速度
    uint8_t returnValue=1;
    uint16_t shootingCircleSpeed=0;
    uint16_t yawPlace=0;
    uint16_t yawDelta=0;
    uint8_t ammo=dartState.ammoState;
    switch (dartState.shootGoal)
    {
    case 0:shootingCircleSpeed=roketShootTask.shootSpeed[ammo];yawPlace=roketShootTask.shootYawPlace[ammo];yawDelta=roketShootTask.miniPcYawDelta[ammo];break;
    case 1:shootingCircleSpeed=roketShootTask.shootSpeedBase[ammo];yawPlace=roketShootTask.shootYawPlaceBase[ammo];yawDelta=roketShootTask.miniPcYawDeltaBase[ammo]; break;
    default:break;
    }
    shootingCircleSetSpeed(shootingCircleSpeed);
    if(minipc.no_data_time>5000||minipc.minipc2mcu.distance==10000){//判定miniPC是否离线,若离线则不调整
        //先调整yaw，再调整推杆，成功则返回1
        if(ammo==0||ammo==2){return ( yawSetPlace(yawPlace) &&pushSetPlace(roketShootTask.shootPushPlace[ammo]) ) ? 1 : 0;}
        else if(ammo==1||ammo==3){return ( yawSetPlace(yawPlace) &&pushGoTop(20000) ) ? 1 : 0;}
    }
    else if(minipc.no_data_time<100){//minipc没离线，使用minipc校准yaw轴
        if(ammo==0||ammo==2){return ( yawSetPlaceVisual(yawDelta) &&pushSetPlace(roketShootTask.shootPushPlace[ammo]) ) ? 1 : 0;}
        else if(ammo==1||ammo==3){return ( yawSetPlaceVisual(yawDelta) &&pushGoTop(20000) ) ? 1 : 0;}
    }
		return 0;
}

uint8_t shootRoket(uint8_t shootingNum,uint8_t refreeNeed){
    uint8_t idx = shootingNum - 1;
    // 缓存全局数组指针，减少后续多次寻址
    const uint16_t *speedArr    = roketShootTask.shootSpeed;
    const uint16_t *yawPlaceArr = roketShootTask.shootYawPlace;
    const uint16_t *yawDeltaArr = roketShootTask.miniPcYawDelta;
    const uint16_t *pushPlaceArr= roketShootTask.shootPushPlace;
    shootingCircleSetSpeed(speedArr[idx]);
    if(!refreeNeed){//是否需要考虑裁判系统，待拓展
        if(minipc.no_data_time>5000||minipc.minipc2mcu.distance==10000){//判定miniPC是否离线,若离线则不调整
            //先调整yaw，再调整推杆，成功则返回1
            if(idx==0||idx==2){return ( yawSetPlace(yawPlaceArr[idx]) &&pushSetPlace(pushPlaceArr[idx]) ) ? 1 : 0;}
            else if(idx==1||idx==3){return ( yawSetPlace(yawPlaceArr[idx]) &&pushGoTop(20000) ) ? 1 : 0;}
        }
        else if(minipc.no_data_time<100){//minipc没离线，使用minipc校准yaw轴
            if(idx==0||idx==2){return ( yawSetPlaceVisual(yawDeltaArr[idx]) &&pushSetPlace(pushPlaceArr[idx]) ) ? 1 : 0;}
            else if(idx==1||idx==3){return ( yawSetPlaceVisual(yawDeltaArr[idx]) &&pushGoTop(20000) ) ? 1 : 0;}
        }
    }
    return 0;
}

void emergencyStop(){
    yawStop();
    shootingCircleStop();
    pushStop();
}

static char word_buf[64];

void manualTask(){
    yawPlaceRefresh();
    dartState.needJiaoZhun=1;
    dartState.semiAutoState=0;
    int16_t yawSpeed=rc_ctrl.rc.ch[2];//左左右
	int16_t pushSpeed=45*rc_ctrl.rc.ch[3];//左上下
    int16_t magazineSpeed=2*rc_ctrl.rc.ch[0];//右左右
    if(abs(yawSpeed)>500){yawSetSpeed(yawSpeed);}else{yawStop();}
    if(abs(pushSpeed)>500){pushSetSpeed(pushSpeed);}else{pushStop();}
    if(rc_ctrl.rc.s[0]==3||dartState.magazineSwitch==1||dartState.magazineSwitch==2){
        shootingCircleSetSpeed(400);}//当换弹和遥控器控制时，令摩擦轮向外旋转，防止此时进行供弹
    else if(rc_ctrl.rc.s[0]==2){
        shootingCircleSetSpeed(-400);}
    else{
        shootingCircleStop();}
    if(magazineSpeed<-500){//切换到弹夹1
        dartState.magazineSwitch=1;
    }
    else if (magazineSpeed>500){//切换到弹夹2
       dartState.magazineSwitch=2;
    }
    magazineSwitch(dartState.magazineSwitch);

    // sprintf(word_buf,"%.4f,%d,%d\n",DWTGetTimeMs(),D3508_motor1.rotor_speed,D3508_motor2.rotor_speed);
    // CDC_Transmit_FS((uint8_t*)word_buf,strlen(word_buf));
}

void semiAutoTask(){
    if(dartState.needJiaoZhun){
        if(pushYawInit()){dartState.semiAutoState=1;}
    }
	else{
        switch (dartState.semiAutoState)
        {
        
        case 0: break;//调试模式
        case 1: if(shootPrepare(1,roketShootTask.shootYawPlace[0],2400)&&rc_ctrl.rc.s[0]==3){dartState.semiAutoState=2;dartState.visualJiaoZhun=0;}break;//第一组弹夹发射准备
        case 2: if(shootRoket(1,0)&&rc_ctrl.rc.s[0]==3){dartState.semiAutoState=3;dartState.visualJiaoZhun=0;}; break;//第一个飞镖发射
        case 3: if(shootRoket(2,0)){dartState.semiAutoState=4;}; break;//第二个飞镖发射
        case 4: if(shootPrepare(2,roketShootTask.shootYawPlace[2],2400)&&rc_ctrl.rc.s[0]==3){dartState.semiAutoState=5;dartState.visualJiaoZhun=0;} break;//第二组弹夹发射准备
        case 5: if(shootRoket(3,0)&&rc_ctrl.rc.s[0]==3){dartState.semiAutoState=6;dartState.visualJiaoZhun=0;}; break;//第三个飞镖发射
        case 6: if(shootRoket(4,0)){dartState.semiAutoState=7;dartState.visualJiaoZhun=0;}; break;//第四个飞镖发射
        case 7: emergencyStop();break;
        default:
            break;
        }
			
	}
} 
void fullAutoTask(){
    if(dartState.needJiaoZhun){
        shootingCircleStop();//比赛前摩擦轮一动不敢动
        if(pushYawInit()){
        dartState.ammoState=0;
        dartState.fullAutoState=0;
        dartState.cangmenState=0;}
    }
    else{
        switch (dartState.fullAutoState)
        {
        case 0:shootingCircleStop();break;//比赛前状态，摩擦轮一动不敢动
        case 1:SCYawPushPrepare(1);break;//第一次发射准备,调整yaw角度至预置位
        case 2:if(roketShootFull()){
            dartState.fullAutoState=3;
            dartState.ammoState=1;
            float nowMs=DWTGetTimeMs();
            minipcDebugDataSend(&D6020_motor1.absolute_angle,&nowMs,&dartState.ammoState,&dartState.visualJiaoZhun);
            dartState.visualJiaoZhun=0;
        }; break;//第一次发射
        case 3:if(SCYawPushPrepareHalf()){dartState.fullAutoState=4;}break;
        case 4:if(roketShootFull()){
            dartState.fullAutoState=5;
            dartState.ammoState=2;
            float nowMs=DWTGetTimeMs();
            minipcDebugDataSend(&D6020_motor1.absolute_angle,&nowMs,&dartState.ammoState,&dartState.visualJiaoZhun);
            dartState.visualJiaoZhun=0;
        }; break;//第二次发射
        case 5:SCYawPushPrepare(1);break;//第三次发射准备
        case 6:if(roketShootFull()){
            dartState.fullAutoState=7;
            dartState.ammoState=3;
            float nowMs=DWTGetTimeMs();
            minipcDebugDataSend(&D6020_motor1.absolute_angle,&nowMs,&dartState.ammoState,&dartState.visualJiaoZhun);
            dartState.visualJiaoZhun=0;
        }; break;//第三次发射
        case 7:if(SCYawPushPrepareHalf()){dartState.fullAutoState=8;}break;//第四次发射准备
        case 8:if(roketShootFull()){
            dartState.fullAutoState=9;
            dartState.ammoState=4;
            float nowMs=DWTGetTimeMs();
            minipcDebugDataSend(&D6020_motor1.absolute_angle,&nowMs,&dartState.ammoState,&dartState.visualJiaoZhun);
            dartState.visualJiaoZhun=0;
        }; break;//第三次发射
        case 9:emergencyStop();break;//比赛结束状态，摩擦轮一动不敢动
        default:
            break;
        }
    }
} 