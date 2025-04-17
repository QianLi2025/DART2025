//c�ļ�
#include "DWT.h"

//�Ĵ�������ַ
#define    DWT_CR    *(uint32_t*)0xE0001000
#define    DWT_CYCCNT    *(uint32_t*)0xE0001004
#define    DEM_CR    *(uint32_t*)0xE000EDFC
 
//������ʹ��λ
#define    DEM_CR_TRCENA    (1<<24)
#define    DWT_CR_CYCCNTENA    (1<<0)

static uint32_t lastTick = 0;    // �ϴε�ʱ���
static uint32_t countCycle = 0;  // ����������

static float DWTNowMs=0;

void DWT_init(void)
{
  DEM_CR |= (uint32_t)DEM_CR_TRCENA;
  DWT_CYCCNT = (uint32_t)0u;
  DWT_CR |= (uint32_t)DWT_CR_CYCCNTENA;
}
//get DWT count
uint32_t DWT_TS_GET(void)
{
  return((uint32_t)DWT_CYCCNT);
}



float DWTGetTimeS(){
	uint32_t nowTick=DWT_TS_GET();
	if(nowTick<lastTick){
		countCycle+=1;
		lastTick = nowTick;
		return (4294967295.0f / 168000000.0f) * (countCycle)+(nowTick/ 168000000.0f);
	}
	else{
		lastTick = nowTick;
		return (4294967295.0f / 168000000.0f) * (countCycle)+(nowTick/ 168000000.0f);
	}
}

void DWTRefreshTimeMs(){
	uint32_t nowTick=DWT_TS_GET();
	if(nowTick<lastTick){
		countCycle+=1;
		lastTick = nowTick;
		DWTNowMs=(4294967295.0f / 168000.0f) * (countCycle)+(nowTick/ 168000.0f);
	}
	else{
		lastTick = nowTick;
		DWTNowMs=(4294967295.0f / 168000.0f) * (countCycle)+(nowTick/ 168000.0f);
	}
}

float DWTGetTimeMs(){
	return DWTNowMs;
}

float DWTGetDeltaTMS(float *lastMS){//�����������һ��ʱ���Dt
	DWTRefreshTimeMs();
	float dt=DWTGetTimeMs()-*lastMS;
	*lastMS=DWTNowMs;
	return dt;
}