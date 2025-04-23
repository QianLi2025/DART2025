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
                        .semiAutoState=1};//系统状态
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
    roketShootTask.shootPushPlace[0]=4100;
    roketShootTask.shootPushPlace[1]=5700;
    roketShootTask.shootPushPlace[2]=4100;
    roketShootTask.shootPushPlace[3]=5700;
    roketShootTask.shootYawPlace[0]=r1->yawPlace;
    roketShootTask.shootYawPlace[1]=r2->yawPlace;
    roketShootTask.shootYawPlace[2]=r3->yawPlace;
    roketShootTask.shootYawPlace[3]=r4->yawPlace;
    roketShootTask.miniPcYawDelta[0]=r1->yawDelta;
    roketShootTask.miniPcYawDelta[1]=r2->yawDelta;
    roketShootTask.miniPcYawDelta[2]=r3->yawDelta;
    roketShootTask.miniPcYawDelta[3]=r4->yawDelta;
}

void checkControlMode(){
    switch (rc_ctrl.rc.s[1])//遥控器选择控制模式
    {
    case 1:mode=fullAuto;break;//全自动模式fuli auto shooter
    case 2:mode=semiAuto;break;//半自动模式
    case 3:mode=manual;break;//手动模式
    default:mode=notConnected;break;
    }
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
		HAL_UART_Transmit(&huart5,rfDataBuf,Size,0xff);
        referee_fbkdata(&rfData,rfDataBuf);
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
    dartState.pushCurrentLimitPushed = (tc1 > 5000) || (tc1 < -5000);
    // 3. 弹夹电机电流超限检测
    int16_t tc2 = D2006_motor2.torque_current;
    dartState.magazineCurrentLimitPushed = (tc2 > 5000) || (tc2 < -5000);
    // 4. Yaw 位置超限：如果角度>3000，强制设为 2
    int16_t yawAng = D6020_motor1.absolute_angle;
    if (yawAng > 3000) {yawPushed = 2;}
    dartState.yawLimitPushed = yawPushed;
    // 5. 只要 yawLimitPushed 为1（触发轻触开关），就清零绝对角度
    if (yawPushed==1) {D6020_motor1.absolute_angle = 0;}
    // 6. 如果下限位触发，则清零推杆电机角度
    if (dartState.pushLowerLimitPushed) {D2006_motor1.absolute_angle = 0;}
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
//对推杆电机进行设置，没有使用位置环
uint8_t pushSetPlace(int16_t position){
    if(D2006_motor1.absolute_angle<position){pushSetSpeed(20000);return 0;}
    else{pushSetSpeed(0);return 1;}
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
    return (abs(D6020_motor1.nowPixel+yawDelta)<=2&&abs(D6020_motor1.set_voltage)<1000) ? 1:0;//当位置达到且速度小于一定时才认为成功置位
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

static char word_buf[64];

void manualTask(){
    yawPlaceRefresh();
    dartState.needJiaoZhun=1;
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
        case 1: if(shootPrepare(1,roketShootTask.shootYawPlace[0],2400)&&rc_ctrl.rc.s[0]==3){dartState.semiAutoState=2;}break;//第一组弹夹发射准备
        case 2: if(shootRoket(1,0)&&rc_ctrl.rc.s[0]==3){dartState.semiAutoState=3;}; break;//第一个飞镖发射
        case 3: if(shootRoket(2,0)){dartState.semiAutoState=4;}; break;//第二个飞镖发射
        case 4: if(shootPrepare(2,roketShootTask.shootYawPlace[2],2400)&&rc_ctrl.rc.s[0]==3){dartState.semiAutoState=5;} break;//第二组弹夹发射准备
        case 5: if(shootRoket(3,0)&&rc_ctrl.rc.s[0]==3){dartState.semiAutoState=6;}; break;//第三个飞镖发射
        case 6: if(shootRoket(4,0)){dartState.semiAutoState=7;}; break;//第四个飞镖发射
        default:
            break;
        }
			
	}
}  