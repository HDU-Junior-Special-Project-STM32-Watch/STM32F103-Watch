/********************************************************************************************************************
* 移植项目
* 链接：【【开源】oled 滚动时钟 3.0 版本，全新字体，代码精简，移植方便】 
* https://www.bilibili.com/video/BV1PpcEz5EUo/?share_source=copy_web&vd_source=befff269c2e0ef1583e2528659770521
********************************************************************************************************************/


#ifndef __ROLL_CLOCK_H__
#define __ROLL_CLOCK_H__


typedef struct {
	
	uint8_t Last;
	uint8_t New;
	
	uint8_t Ge_Last;
	uint8_t Ge_New;
	
	uint8_t Shi_Last;
	uint8_t Shi_New;
	
	uint8_t Ge_Roll_Flag;
	uint8_t Shi_Roll_Flag;
	
	int8_t Ge_Y;
	int8_t Shi_Y;
	
}Clock;

void 	Roll_Clock_Init	(void);
void 	Roll_Clock		(void);

#endif
