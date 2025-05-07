#ifndef MINIPC_PROTOCOL_H
#define MINIPC_PROTOCOL_H

#include "stdint.h"
#include "string.h"
#include "CRC8_CRC16.h"
#include "DWT.h"



typedef struct
{
    uint8_t mcu2minipc_buf[100];
    float no_data_time;
    float pack_loss_rate;
    #pragma pack(1)
    struct//__packed 要求整个结构体定义依字节对齐，如果没有这个则会在内存中依字（4字节）对齐
    {
        uint8_t header; // = 0x5A;
        uint8_t ammo;//第几发飞镖：原is_Rune
        uint8_t isVisual; //是否使用了视觉校准？ detect_color
        uint8_t is_reset;
        float time;  // time(ms):原roll
        float yaw;   // yaw(unkown)
        float pitch; //pitch
        float v0;//原v0
        float motor_speed;
        uint16_t checksum; // = 0; // c! 16位crc校验
    } mcu2minipc;
    struct
    {
        uint8_t header; // = 0xA5;
        uint8_t is_tracking;
        uint8_t is_can_hit;
        float yaw;
        float pitch;
        float distance;
        uint16_t checksum; // = 0;
    } minipc2mcu;
    #pragma pack()
    float minipc_count;      //用于记时
    uint8_t rx_pack_state[100]; //记录当前及100个数据包中的接收状态，0什么都不表示，1表示接收正常，2表示丢包
} minipc_t;

extern minipc_t minipc; //定义一个全局变量，存放数据包

void minipc_rec(minipc_t *pc, uint8_t pc_data_buf[]);
void minipc_upgrade(minipc_t *pc);

#endif // MINIPC_PROTOCOL_H
