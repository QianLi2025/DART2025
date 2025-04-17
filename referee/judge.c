#include "judge.h"
#include "crc_ref.h"
#include "main.h"


#include "struct_typedef.h"
#include "string.h"

JUDGE_MODULE_DATA Judge_Hero;

uint8_t Hero_level;

uint8_t Hero_id;//本机ID

uint16_t Hero_42mm_speed_limit;
uint16_t Hero_chassis_power_limit;
uint16_t Hero_chassis_power_buffer;
float Hero_chassis_power;
float Hero_42mm_speed;

uint8_t Flag_progress;//比赛阶段，4：正在比赛
uint8_t Flag_judge = 0;//己方颜色，1：红，2：蓝

uint8_t Flag_first = 0;
//extern uint8_t first_x;
//extern uint8_t first_y;
//extern CAN_HandleTypeDef hcan1;

uint8_t Flag_target = 1;//打击目标，1：前哨站，2：基地
uint8_t Flag_SHOOT_OK;//1:可发射，0：禁止发射

void Update_data();//����һЩ��Ҫ�õ��ı�����ʵʱ������ֵ���������ļ�����

void JUDGE_Receive(volatile uint8_t *databuffer,uint8_t length)
{
    uint8_t pos=0;
    uint16_t data_length=0;
    uint16_t CMD_ID =0;
    
     while(pos<length)
     {
        if(databuffer[pos]==0xA5)
        {
            if(Verify_CRC8_Check_Sum(&databuffer[pos],5))
            {
                data_length = (databuffer[pos+1]&0xff)|((databuffer[pos+2]<<8)&0xff00);
                if(pos+data_length+5+2+2 > length)
                {
                    continue;
                }
            if(Verify_CRC16_Check_Sum(&databuffer[pos],data_length+9))
            {
              
             
                CMD_ID = (databuffer[pos+5]&0xff)|((databuffer[pos+6]<<8)&0xff00);
                switch(CMD_ID)
                { 
									/***************************************************************************************************/
                    case 0x0001://比赛状态数据
                        data_length = 11;
                        memcpy((void*)(&Judge_Hero.status), (const void*)(&databuffer[pos+7]), data_length);
                        break;
									/***************************************************************************************************/
										
                  /***************************************************************************************************/
										case 0x0003://机器人血量数据
                        data_length = 32;
                         memcpy((void*)(&Judge_Hero.robot_hp), (const void*)(&databuffer[pos+7]), data_length);
                         break;
									/***************************************************************************************************/
										
                    case 0x0005:
                        data_length = 13;
                          memcpy((void*)(&Judge_Hero.zone), (const void*)(&databuffer[pos+7]), data_length);
                        break;
                    case 0x0101:
                        data_length = 4;
                        memcpy((void*)(&Judge_Hero.event_data), (const void*)(&databuffer[pos+7]), data_length);
                        break;
                    case 0x0102:
                        data_length = 4;
                         memcpy((void*)(&Judge_Hero.supply_status), (const void*)(&databuffer[pos+7]), data_length);
                         break;
                    case 0x0104 :
                        data_length = 2;
                        memcpy((void*)(&Judge_Hero.warning), (const void*)(&databuffer[pos+7]), data_length);
                        break;
										
										/***************************************************************************************************/
                    case 0x0105 ://飞镖发射相关数据(飞镖剩余时间)
                        data_length = 4;
                        memcpy((void*)(&Judge_Hero.dart_info_t), (const void*)(&databuffer[pos+7]), data_length);
                         break;
										/***************************************************************************************************/
                   
										/***************************************************************************************************/
										case 0x0201://机器人性能体系数据(获取本机器人 ID)
                         data_length = 27;
                         memcpy((void*)(&Judge_Hero.robot_status), (const void*)(&databuffer[pos+7]), data_length);   //���̹���������������
                        break;
										/***************************************************************************************************/
                    
										case 0x0202:
                        data_length = 16;
                        memcpy((void*)(&Judge_Hero.power_heat), (const void*)(&databuffer[pos+7]), data_length);      //��ʵʱ������������
                        break;
                    case 0x0203:
                        data_length = 16;
                         memcpy((void*)(&Judge_Hero.robot_pos), (const void*)(&databuffer[pos+7]), data_length);
                         break;
                    case 0x0204:
                        data_length = 1;
                        memcpy((void*)(&Judge_Hero.buff), (const void*)(&databuffer[pos+7]), data_length);
                        break;
                    case 0x0205:
                        data_length = 1;
                        memcpy((void*)(&Judge_Hero.aerial_energy), (const void*)(&databuffer[pos+7]), data_length);
                        break;
                    case 0x0206:
                        data_length =1;
                        memcpy((void*)(&Judge_Hero.robot_hurt), (const void*)(&databuffer[pos+7]), data_length);
                        break;
                    case 0x0207:
                        data_length = 7;
                        memcpy((void*)(&Judge_Hero.shoot_data), (const void*)(&databuffer[pos+7]), data_length);
                        break;
                    case 0x0208:
                        data_length = 6;
                        memcpy((void*)(&Judge_Hero.bullet_remain), (const void*)(&databuffer[pos+7]), data_length);
                        break;
                    case 0x0209:
                        data_length = 4;
                        memcpy((void*)(&Judge_Hero.rfid_status), (const void*)(&databuffer[pos+7]), data_length);
                        break;
//                    case 0x020A://飞镖选手端指令
//                        data_length = 6;
//                        memcpy((void*)(&Judge_Hero.rfid_status), (const void*)(&databuffer[pos+7]), data_length);
//                        break;
										
										/***************************************************************************************************/
										 case 0x020A://飞镖选手端指令
                        data_length = 6;
                        memcpy((void*)(&Judge_Hero.dart_client_cmd), (const void*)(&databuffer[pos+7]), data_length);
                        break;
									 /***************************************************************************************************/
                    default:break;
                }
                pos+=(data_length+9);
                continue;

            }


          }

        }

        pos++;
     
     }
		 Update_data();
		 
}


void Update_data()
{
	Hero_id = Judge_Hero.robot_status.robot_id;//ID��
//	Hero_level = Judge_Hero.robot_status.robot_level;//�ȼ�
//	Hero_42mm_speed_limit = Judge_Hero.robot_status.shooter_barrel_heat_limit;//42mm������������
//	Hero_chassis_power_limit = Judge_Hero.robot_status.chassis_power_limit;//��������
//	Hero_chassis_power_buffer = Judge_Hero.power_heat.chassis_power_buffer;//��������
//	Hero_chassis_power = Judge_Hero.power_heat.chassis_power;//ʵʱ����
//	if(Judge_Hero.shoot_data.bullet_speed)
//	{
//		Hero_42mm_speed = Judge_Hero.shoot_data.bullet_speed;
//	}
//	//��������
	
	Flag_progress =  Judge_Hero.status.game_progress;//比赛阶段，4：正在比赛	
	
//	if(Flag_progress == 4 && Flag_first == 0)		//������ʼ
//	{
////		first_x = 1;
////		first_y = 1;
//		Flag_first = 1;
////		HAL_TIM_Base_Start_IT(&htim8);
//	}
//	//�ж��ҷ��Ǻ췽��������
//	if(Hero_id == 7)//��ɫ��
//	{
//		Flag_judge = 1;
//	}
//	else if(Hero_id == 107)
//	{
//		Flag_judge = 2;
//	}
	//确认己方颜色，1：红方，2：蓝方
	if(Hero_id == 8)
	{
		Flag_judge = 1;
	}
	else if(Hero_id == 108)
	{
		Flag_judge = 2;
	}
	
//	//�ж��Ǻ췽�ڱ���������,��Ѫǿ�ƿ�����תģʽ
//	if(Flag_judge == 1)//��ɫ��
//	{
//		if(Judge_Hero.robot_hp.red_7_robot_HP!= 0 && Judge_Hero.robot_hp.red_7_robot_HP != 600)
//		{
//			Flag_first = 2;
//		}
//	}
//	
//	else if(Flag_judge == 2)//��ɫ��
//	{
//		if(Judge_Hero.robot_hp.blue_7_robot_HP!= 0 && Judge_Hero.robot_hp.blue_7_robot_HP != 600)
//		{
//			Flag_first = 2;
//		}
//	}
	
	
	
		//判断应该读取哪一座前哨站的血量
	if(Flag_judge == 1)//己方为红方，则依据蓝方前哨站血量判断
	{
			if(Judge_Hero.robot_hp.blue_outpost_HP != 0)
			{
				Flag_target = 1;
			}
			else
			{
				Flag_target = 2;
			}
	}	
	else if(Flag_judge == 2)//己方为蓝方，则依据红方前哨站血量判断
	{
		if(Judge_Hero.robot_hp.red_outpost_HP != 0 )
		{
			Flag_target = 1;
		}
		else
		{
			Flag_target = 2;
		}
	}
	
	if(Judge_Hero.dart_client_cmd.dart_launch_opening_status == 0 & Judge_Hero.dart_info_t.dart_remaining_time > 0)
		//需要注意：dart_launch_opening_status==0为开启，若裁判系统飞镖发射井离线，该值初始化为零会直接判断为闸门已开启，故加入剩余可发射时间
	{
		Flag_SHOOT_OK = 1;		
	}
	else
	{
		Flag_SHOOT_OK = 0;		
	}
}
