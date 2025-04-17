//头文件
#ifndef DWT_H
#define DWT_H

#include <stdint.h>

// 初始化 DWT 计数器
void DWT_init(void);

// 获取当前 DWT 计数器的值
uint32_t DWT_TS_GET(void);

// 获取从上次调用以来的时间（单位：秒）
float DWTGetTimeS();
//刷新毫秒计数
void DWTRefreshTimeMs();
// 获取从上次调用以来的时间（单位：毫秒）
float DWTGetTimeMs();
//返回相较于传入时间的时间（单位：毫秒）,同时会对传入的地址进行赋值
float DWTGetDeltaTMS(float *lastMS);
#endif // DWT_H