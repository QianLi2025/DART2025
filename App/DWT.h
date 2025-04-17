//ͷ�ļ�
#ifndef DWT_H
#define DWT_H

#include <stdint.h>

// ��ʼ�� DWT ������
void DWT_init(void);

// ��ȡ��ǰ DWT ��������ֵ
uint32_t DWT_TS_GET(void);

// ��ȡ���ϴε���������ʱ�䣨��λ���룩
float DWTGetTimeS();
//ˢ�º������
void DWTRefreshTimeMs();
// ��ȡ���ϴε���������ʱ�䣨��λ�����룩
float DWTGetTimeMs();
//��������ڴ���ʱ���ʱ�䣨��λ�����룩,ͬʱ��Դ���ĵ�ַ���и�ֵ
float DWTGetDeltaTMS(float *lastMS);
#endif // DWT_H