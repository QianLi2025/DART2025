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
                        .semiAutoState=0};//系统状态
                      


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
    switch (dartState.yawLimitPushed)
    {
    case 1:
    if(D6020_motor1.set_voltage>0||D6020_motor1.target_speed>0){
        D6020_motor1.set_voltage=0;D6020_motor1.target_speed=0;
        pidReset(&D6020_motor1.motor_speed_pid);
        D6020_motor1.set_voltage=pid_calc(&D6020_motor1.motor_speed_pid,D6020_motor1.target_speed,D6020_motor1.rotor_speed);
    }  
        break;
    case 2:
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
void calAndSendMotor(){//计算和发送电机控制量,
    D2006_motor1.set_voltage=pid_calc(&D2006_motor1.motor_speed_pid, D2006_motor1.target_speed, D2006_motor1.rotor_speed);
    D2006_motor2.set_voltage=pid_calc(&D2006_motor2.motor_speed_pid, D2006_motor2.target_speed, D2006_motor2.rotor_speed);
    D3508_motor1.set_voltage=pid_calc(&D3508_motor1.motor_speed_pid, D3508_motor1.target_speed, D3508_motor1.rotor_speed);
    D3508_motor2.set_voltage=pid_calc(&D3508_motor2.motor_speed_pid, D3508_motor2.target_speed, D3508_motor2.rotor_speed);
    if(D6020_motor1.target_pos==-255){D6020_motor1.set_voltage=pid_calc(&D6020_motor1.motor_speed_pid, D6020_motor1.target_speed, D6020_motor1.rotor_speed);}
    else{
        D6020_motor1.target_speed=-pid_calc(&D6020_motor1.motor_pos_pid,D6020_motor1.target_pos,D6020_motor1.absolute_angle);
        D6020_motor1.set_voltage=pid_calc(&D6020_motor1.motor_speed_pid,D6020_motor1.target_speed,D6020_motor1.rotor_speed);
    }
    limitProtection();
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

void dartSysStateCheck(){
    dartState.yawLimitPushed=yawLimitCheck();
    dartState.pushUpperLimitPushed=pushUpperLimitCheck();
    dartState.pushLowerLimitPushed=pushLowerLimitCheck();
    dartState.pushCurrentLimitPushed=(abs(D2006_motor1.torque_current)>5000);
    dartState.magazineCurrentLimitPushed=(abs(D2006_motor2.torque_current)>5000);
    if(!dartState.needJiaoZhun&&D6020_motor1.absolute_angle>3000){dartState.yawLimitPushed=2;}
}

void yawStop(){D6020_motor1.target_speed=0;D6020_motor1.target_pos=-255;pidReset(&D6020_motor1.motor_speed_pid);}//yaw电机停止
void pushStop(){ D2006_motor1.target_speed=0;D2006_motor1.target_pos=-255;}//推杆电机停止
void magazineStop(){ D2006_motor2.target_speed=0;}//弹夹电机停止
void shootingCircleStop(){D3508_motor1.target_speed=0;D3508_motor2.target_speed=0;} //摩擦轮停止
void yawSetSpeed(int16_t speed){D6020_motor1.target_speed=-speed;D6020_motor1.target_pos=-255;}
void pushSetSpeed(int16_t speed){D2006_motor1.target_speed=-speed;D2006_motor1.target_pos=-255;}
void magazineSetSpeed(int16_t speed){D2006_motor2.target_speed=speed;}
void shootingCircleSetSpeed(int16_t speed){D3508_motor1.target_speed=-speed;D3508_motor2.target_speed=speed;}


void pushYawInit(){
   dartSysStateCheck();
   if(!dartState.pushLowerLimitPushed){pushSetSpeed(-30000);}else{pushStop();}
   if(!dartState.yawLimitPushed){yawSetSpeed(-8000);}else{yawStop();}
   if(dartState.pushLowerLimitPushed && dartState.yawLimitPushed){
       pushStop();yawStop();
       D6020_motor1.absolute_angle=0;D6020_motor1.last_angle=D6020_motor1.rotor_angle;//为计算绝对距离做准备
       D2006_motor1.absolute_angle=0;D2006_motor1.last_angle=DWTGetTimeMs();//为计算绝对距离做准备
       dartState.needJiaoZhun=0;
       return;
   }
}


void yawPlaceRefresh(){// 对yaw轴电机（D6020motor1）的绝对位置进行更新
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
void pushSetPlace(int16_t position){
    pushPlaceRefreshSpeedy();
    if(D2006_motor1.absolute_angle<position){pushSetSpeed(20000);}
    else{pushSetSpeed(0);}
}
void yawSetPlace(int16_t position){
    yawPlaceRefresh();
    D6020_motor1.target_pos=position;
}

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

void shootPrepare(uint8_t shootingMagazine,int16_t yawSet,int16_t pushSet){
    magazineSwitch(shootingMagazine);//使用哪组弹夹射击？
	yawSetPlace(yawSet);//yaw轴预置位是？
    if(dartState.magazineSwitch==10*shootingMagazine+1){pushSetPlace(pushSet);}
    return;
}

void manualTask(){
    yawPlaceRefresh();
    dartState.needJiaoZhun=1;
    int16_t yawSpeed=rc_ctrl.rc.ch[2];//左左右
	int16_t pushSpeed=45*rc_ctrl.rc.ch[3];//左上下
    int16_t magazineSpeed=2*rc_ctrl.rc.ch[0];//右左右
    if(abs(yawSpeed)>500){yawSetSpeed(yawSpeed);}else{yawStop();}
    if(abs(pushSpeed)>500){pushSetSpeed(pushSpeed);}else{pushStop();}
    if(rc_ctrl.rc.s[0]==3){shootingCircleSetSpeed(100);}else(shootingCircleStop());
    if(magazineSpeed<-500){//切换到弹夹2
        dartState.magazineSwitch=2;
    }
    else if (magazineSpeed>500){//切换到弹夹1
       dartState.magazineSwitch=1;
    }
    magazineSwitch(dartState.magazineSwitch);
}

void semiAutoTask(){
    if(dartState.needJiaoZhun){
        pushYawInit();
    }
	else{
        switch (dartState.semiAutoState)
        {
        //调试模式
        case 0: shootPrepare(1,1000,10);break;
        case 1: break;
        default:
            break;
        }
			
	}
}  