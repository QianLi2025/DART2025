// #include "referee.h"

// referee_t rfData;
// uint8_t rfDataBuf[150]={0};

// void referee_fbkdata(referee_t *rf, uint8_t buf[])
// {
//     int data_len = 0;
//     int data_pack_len = 0;
//     uint16_t cmd_id = 0;
//     if (Verify_CRC8_Check_Sum(buf, sizeof(frame_header_t)))
//     {
//         memcpy((void *)&(rf->frame_header), (const void *)buf, sizeof(frame_header_t));
//         if (rf->frame_header.SOF == 0xa5)
//         {
//             data_len = rf->frame_header.data_length;
//             data_pack_len = 5 + 2 + data_len + 2;
//             if (Verify_CRC16_Check_Sum(buf, data_pack_len))
//             {
//                 memcpy((void *)&(cmd_id), (const void *)&buf[sizeof(frame_header_t)], sizeof(uint16_t));
//                 switch (cmd_id)
//                 {
//                 case 0x001:
//                     memcpy((void *)&(rf->game_status), (const void *)&buf[sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->game_status));
//                     break;
//                 case 0x002:
//                     memcpy((void *)&(rf->game_result), (const void *)&buf[sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->game_result));
//                     break;
//                 case 0x003:
//                     memcpy((void *)&(rf->game_robot_HP), (const void *)&buf[sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->game_robot_HP));
//                     break;
//                 case 0x101:
//                     memcpy((void *)&(rf->event_data), (const void *)&buf[sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->event_data));
//                     break;
//                 case 0x104:
//                     memcpy((void *)&(rf->referee_warning), (const void *)&buf[sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->referee_warning));
//                     break;
//                 case 0x105:
//                     memcpy((void *)&(rf->dart_info), (const void *)&buf[sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->dart_info));
//                     break;
//                 case 0x201:
//                     memcpy((void *)&(rf->robot_status), (const void *)&buf[sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->robot_status));
//                     break;
//                 case 0x202:
//                     memcpy((void *)&(rf->power_heat_data), (const void *)&buf[sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->power_heat_data));
//                     break;
//                 case 0x203:
//                     memcpy((void *)&(rf->robot_pos), (const void *)&buf[sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->robot_pos));
//                     break;
//                 case 0x204:
//                     memcpy((void *)&(rf->buff), (const void *)&buf[sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->buff));
//                     break;
//                 case 0x206:
//                     memcpy((void *)&(rf->hurt_data), (const void *)&buf[sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->hurt_data));
//                     break;
//                 case 0x207:
//                     memcpy((void *)&(rf->shoot_data), (const void *)&buf[sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->shoot_data));
//                     break;
//                 case 0x208:
//                     memcpy((void *)&(rf->projectile_allowance), (const void *)&buf[sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->projectile_allowance));
//                     break;
//                 case 0x209:
//                     memcpy((void *)&(rf->rfid_status), (const void *)&buf[sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->rfid_status));
//                     break;
//                 case 0x20a:
//                     memcpy((void *)&(rf->dart_client_cmd), (const void *)&buf[sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->dart_client_cmd));
//                     break;
//                 case 0x20b:
//                     memcpy((void *)&(rf->ground_robot_position), (const void *)&buf[sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->ground_robot_position));
//                     break;
//                 case 0x20c:
//                     memcpy((void *)&(rf->radar_mark_data), (const void *)&buf[sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->radar_mark_data));
//                     break;
//                 case 0x20d:
//                     memcpy((void *)&(rf->sentry_info), (const void *)&buf[sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->sentry_info));
//                     break;
//                 case 0x20e:
//                     memcpy((void *)&(rf->radar_info), (const void *)&buf[sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->radar_info));
//                     break;
//                 case 0x302:
//                     memcpy((void *)&(rf->custom_robot_data), (const void *)&buf[sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->custom_robot_data));
//                     break;
//                 case 0x303:
//                     memcpy((void *)&(rf->map_command), (const void *)&buf[sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->map_command));
//                     break;
//                 case 0x304:
//                     memcpy((void *)&(rf->remote_control), (const void *)&buf[sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->remote_control));
//                     break;
//                 case 0x305:
//                     memcpy((void *)&(rf->map_robot_data), (const void *)&buf[sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->map_robot_data));
//                     break;
//                 case 0x306:
//                     memcpy((void *)&(rf->custom_client_data), (const void *)&buf[sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->custom_client_data));
//                     break;
//                 case 0x307:
//                     memcpy((void *)&(rf->map_data), (const void *)&buf[sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->map_data));
//                     break;
//                 case 0x308:
//                     memcpy((void *)&(rf->custom_info), (const void *)&buf[sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->custom_info));
//                     break;
//                 case 0x309:
//                     memcpy((void *)&(rf->robot_custom_data), (const void *)&buf[sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->robot_custom_data));
//                     break;
//                 default:
//                     break;
//                 }
//             }
//         }
//     }
// }
#include "referee.h"

// void referee_init(referee_data_t *rf, int default_id, referee_cm_t *rf_cm, int device_index)
// {
//     memset(rf, 0, sizeof(referee_data_t));
//     rf->robot_status.robot_id = default_id;
//     rf->power_heat_data.buffer_energy = 60;
//     rf->robot_status.chassis_power_limit = 45; //默认功率上限
//     //根据裁判系统发送数据的周期的5倍定义超时时间
//     //共用一个设备号
//     //只初始化关键数据监视器
//     // device_init(&(rf_cm->game_status_cm), 5, device_index);
//     // device_init(&(rf_cm->game_result_cm), 10000000000000, device_index);
//     // device_init(&(rf_cm->game_robot_HP_cm), 2, device_index);
//     // device_init(&(rf_cm->event_data_cm), 5, device_index);
//     // device_init(&(rf_cm->referee_warning_cm), 5, device_index);
//     // device_init(&(rf_cm->dart_info_cm), 5, device_index);
//     device_init(&(rf_cm->robot_status_cm), 0.5, device_index);
//     device_init(&(rf_cm->power_heat_data_cm), 0.5, device_index);
// }

referee_t rfData;
uint8_t rfDataBuf[150]={0};

void referee_fbkdata(referee_t *rf,uint8_t buf[], int max_data_len)
{
    int data_len = 0;
    int data_pack_len = 0;
    uint16_t cmd_id = 0;
    int pos = 0; //处理粘包
    while (pos < max_data_len)
    {
        if (buf[pos] == 0xa5)
        {
            if (Verify_CRC8_Check_Sum(&(buf[pos]), sizeof(frame_header_t)))
            {
                memcpy((void *)&(rf->frame_header), (const void *)&(buf[pos]), sizeof(frame_header_t));
                data_len = rf->frame_header.data_length;
                data_pack_len = 5 + 2 + data_len + 2;
                if (pos + data_pack_len > max_data_len) //搜到底了
                    break;
                if (Verify_CRC16_Check_Sum(&(buf[pos]), data_pack_len))
                {
                    memcpy((void *)&(cmd_id), (const void *)&buf[pos + sizeof(frame_header_t)], sizeof(uint16_t));
                    switch (cmd_id)
                    {
                    case 0x001:
                        memcpy((void *)&(rf->game_status), (const void *)&buf[pos + sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->game_status));
                        break;
                    case 0x002:
                        memcpy((void *)&(rf->game_result), (const void *)&buf[pos + sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->game_result));
                        break;
                    case 0x003:
                        memcpy((void *)&(rf->game_robot_HP), (const void *)&buf[pos + sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->game_robot_HP));
                        break;
                    case 0x101:
                        memcpy((void *)&(rf->event_data), (const void *)&buf[pos + sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->event_data));
                        break;
                    case 0x104:
                        memcpy((void *)&(rf->referee_warning), (const void *)&buf[pos + sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->referee_warning));
                        break;
                    case 0x105:
                        memcpy((void *)&(rf->dart_info), (const void *)&buf[pos + sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->dart_info));
                        break;
                    case 0x201:
                        memcpy((void *)&(rf->robot_status), (const void *)&buf[pos + sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->robot_status));
                        break;
                    case 0x202:
                        memcpy((void *)&(rf->power_heat_data), (const void *)&buf[pos + sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->power_heat_data));
                        break;
                    case 0x203:
                        memcpy((void *)&(rf->robot_pos), (const void *)&buf[pos + sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->robot_pos));
                        break;
                    case 0x204:
                        memcpy((void *)&(rf->buff), (const void *)&buf[pos + sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->buff));
                        break;
                    case 0x206:
                        memcpy((void *)&(rf->hurt_data), (const void *)&buf[pos + sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->hurt_data));
                        break;
                    case 0x207:
                        memcpy((void *)&(rf->shoot_data), (const void *)&buf[pos + sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->shoot_data));
                        break;
                    case 0x208:
                        memcpy((void *)&(rf->projectile_allowance), (const void *)&buf[pos + sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->projectile_allowance));
                        break;
                    case 0x209:
                        memcpy((void *)&(rf->rfid_status), (const void *)&buf[pos + sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->rfid_status));
                        break;
                    case 0x20a:
                        memcpy((void *)&(rf->dart_client_cmd), (const void *)&buf[pos + sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->dart_client_cmd));
                        break;
                    case 0x20b:
                        memcpy((void *)&(rf->ground_robot_position), (const void *)&buf[pos + sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->ground_robot_position));
                        break;
                    case 0x20c:
                        memcpy((void *)&(rf->radar_mark_data), (const void *)&buf[pos + sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->radar_mark_data));
                        break;
                    case 0x20d:
                        memcpy((void *)&(rf->sentry_info), (const void *)&buf[pos + sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->sentry_info));
                        break;
                    case 0x20e:
                        memcpy((void *)&(rf->radar_info), (const void *)&buf[pos + sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->radar_info));
                        break;
                    case 0x301:
                        //robot_data_fbk(&buf[pos + sizeof(frame_header_t) + sizeof(uint16_t)], data_len);
                        break;
                    case 0x302:
                        memcpy((void *)&(rf->custom_robot_data), (const void *)&buf[pos + sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->custom_robot_data));
                        break;
                    case 0x303:
                        memcpy((void *)&(rf->map_command), (const void *)&buf[pos + sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->map_command));
                        break;
                    case 0x304:
                        memcpy((void *)&(rf->remote_control), (const void *)&buf[pos + sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->remote_control));
                        break;
                    case 0x305:
                        memcpy((void *)&(rf->map_robot_data), (const void *)&buf[pos + sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->map_robot_data));
                        break;
                    case 0x306:
                        memcpy((void *)&(rf->custom_client_data), (const void *)&buf[pos + sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->custom_client_data));
                        break;
                    case 0x307:
                        memcpy((void *)&(rf->map_data), (const void *)&buf[pos + sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->map_data));
                        break;
                    case 0x308:
                        memcpy((void *)&(rf->custom_info), (const void *)&buf[pos + sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->custom_info));
                        break;
                    case 0x309:
                        memcpy((void *)&(rf->robot_custom_data), (const void *)&buf[pos + sizeof(frame_header_t) + sizeof(uint16_t)], sizeof(rf->robot_custom_data));
                        break;
                    default:
                        break;
                    }
                    pos += data_pack_len; //更新处理缓冲区的编号
                    continue;
                }
            }
        }
        pos++;
    }
}

// void referee_refresh(referee_cm_t *rf_cm)
// {
//     device_refresh(&(rf_cm->robot_status_cm));
//     device_refresh(&(rf_cm->power_heat_data_cm));
// }
