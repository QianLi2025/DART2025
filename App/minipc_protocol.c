#include "minipc_protocol.h"

minipc_t minipc={.mcu2minipc.ammo=0,
                 .mcu2minipc.header=0X5A,
                 .mcu2minipc.isVisual=0,
                 .mcu2minipc.is_reset=0,
                 .mcu2minipc.time=0,
                 .mcu2minipc.yaw=0,
                 .mcu2minipc.pitch=0,
                 .mcu2minipc.v0=0,
                 .mcu2minipc.motor_speed=0}; //定义一个全局变量，存放数据包

//在接收数据的回调中使用，接收数据到缓冲区，并清空no_data_time
void minipc_rec(minipc_t *pc, uint8_t pc_data_buf[])
{
    //移位
    for (int i = 0; i < 99; i++)
        pc->rx_pack_state[99 - i] = pc->rx_pack_state[98 - i];
    // crc校验
    if (Verify_CRC16_Check_Sum(pc_data_buf, sizeof(pc->minipc2mcu)))
    {
        pc->rx_pack_state[0] = 1;
        memcpy((void *)&(pc->minipc2mcu), (const void *)pc_data_buf, sizeof(pc->minipc2mcu));
    }
    else
        pc->rx_pack_state[0] = 2;
    pc->no_data_time = 0;
    DWTGetDeltaTMS(&(pc->minipc_count)); //更新记时点
}
//放在循环中，更新no_data_time和pack_loss_rate，并将mcu数据附上crc校验，更新发送缓冲区
void minipc_upgrade(minipc_t *pc)
{
    float dt = DWTGetDeltaTMS(&(pc->minipc_count));
    pc->no_data_time += dt;
    int pack_ok = 0;
    int pack_loss = 0;
    for (int i = 0; i < 100; i++)
    {
        if (pc->rx_pack_state[i] == 1)
            pack_ok++;
        else if (pc->rx_pack_state[i] == 2)
            pack_loss++;
        else
            break;
    }
    if ((pack_loss + pack_ok) > 0)
        pc->pack_loss_rate = ((float)pack_loss) / ((float)(pack_loss + pack_ok));
    pc->mcu2minipc.header = 0x5a;
    Append_CRC16_Check_Sum((uint8_t *)&(pc->mcu2minipc), sizeof(pc->mcu2minipc));
    memcpy((void *)(pc->mcu2minipc_buf), (const void *)&(pc->mcu2minipc), sizeof(pc->mcu2minipc));
}
