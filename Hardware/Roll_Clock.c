/********************************************************************************************************************
* 移植项目
* 链接：【【开源】oled 滚动时钟 3.0 版本，全新字体，代码精简，移植方便】 
* https://www.bilibili.com/video/BV1PpcEz5EUo/?share_source=copy_web&vd_source=befff269c2e0ef1583e2528659770521
********************************************************************************************************************/


#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MyRTC.h"
#include "Roll_Clock.h"


Clock Sec;
Clock Min;
Clock Hour;

void Roll_Clock_Init(void)
{
	MyRTC_ReadTime();
	Sec.Last=MyRTC_Time[5];
	Min.Last=MyRTC_Time[4];
	Hour.Last=MyRTC_Time[3];
	OLED_ShowChar(42,24,':',OLED_8X16);
	OLED_ShowChar(78,24,':',OLED_8X16);
}

void OLED_Roll(void)
{
	Sec.Ge_Y=16;
	Sec.Shi_Y=16;
	
	Min.Ge_Y=16;
	Min.Shi_Y=16;
	
	Hour.Ge_Y=16;
	Hour.Shi_Y=16;
	
	for (uint8_t i=0;i<=24;i++)
	{
		OLED_ShowChar(40,16,':',OLED_12X24);
		OLED_ShowChar(76,16,':',OLED_12X24);
		
		OLED_ShowNum(100,Sec.Ge_Y,Sec.Ge_Last,1,OLED_12X24);
		OLED_ShowNum(100,Sec.Ge_Y+24,Sec.Ge_New,1,OLED_12X24);
		
		OLED_ShowNum(88,Sec.Shi_Y,Sec.Shi_Last,1,OLED_12X24);
		OLED_ShowNum(88,Sec.Shi_Y+24,Sec.Shi_New,1,OLED_12X24);
		
		
		OLED_ShowNum(64,Min.Ge_Y,Min.Ge_Last,1,OLED_12X24);
		OLED_ShowNum(64,Min.Ge_Y+24,Min.Ge_New,1,OLED_12X24);
		
		OLED_ShowNum(52,Min.Shi_Y,Min.Shi_Last,1,OLED_12X24);
		OLED_ShowNum(52,Min.Shi_Y+24,Min.Shi_New,1,OLED_12X24);
		
		
		OLED_ShowNum(28,Hour.Ge_Y,Hour.Ge_Last,1,OLED_12X24);
		OLED_ShowNum(28,Hour.Ge_Y+24,Hour.Ge_New,1,OLED_12X24);
		
		OLED_ShowNum(16,Hour.Shi_Y,Hour.Shi_Last,1,OLED_12X24);
		OLED_ShowNum(16,Hour.Shi_Y+24,Hour.Shi_New,1,OLED_12X24);
		
		OLED_UpdateArea(16, 16, 96, 24);
		
		if (Sec.Ge_Roll_Flag==1)
		{
			Sec.Ge_Y--;
		}
		if (Sec.Shi_Roll_Flag==1)
		{
			Sec.Shi_Y--;
		}
		if (Min.Ge_Roll_Flag==1)
		{
			Min.Ge_Y--;
		}
		if (Min.Shi_Roll_Flag==1)
		{
			Min.Shi_Y--;
		}
		if (Hour.Ge_Roll_Flag==1)
		{
			Hour.Ge_Y--;
		}
		if (Hour.Shi_Roll_Flag==1)
		{
			Hour.Shi_Y--;
		}
	}
	Sec.Ge_Roll_Flag=0;
	Sec.Shi_Roll_Flag=0;
	Min.Ge_Roll_Flag=0;
	Min.Shi_Roll_Flag=0;
	Hour.Ge_Roll_Flag=0;
	Hour.Shi_Roll_Flag=0;
}

void Roll_Clock()
{
		Sec.New=MyRTC_Time[5];
		Min.New=MyRTC_Time[4];
		Hour.New=MyRTC_Time[3];
		if (Sec.Last!=Sec.New)
		{
			Sec.Ge_Last=Sec.Last%10;
			Sec.Ge_New=Sec.New%10;
			Sec.Shi_Last=Sec.Last/10;
			Sec.Shi_New=Sec.New/10;
		
			Min.Ge_Last=Min.Last%10;
			Min.Ge_New=Min.New%10;
			Min.Shi_Last=Min.Last/10;
			Min.Shi_New=Min.New/10;
			
			Hour.Ge_Last=Hour.Last%10;
			Hour.Ge_New=Hour.New%10;
			Hour.Shi_Last=Hour.Last/10;
			Hour.Shi_New=Hour.New/10;
			
			Sec.Ge_Roll_Flag=1;
			
			if (Sec.Ge_Last==9 && Sec.Ge_New==0) Sec.Shi_Roll_Flag=1;
			
			if (Sec.Shi_Last==5 && Sec.Shi_New==0) Min.Ge_Roll_Flag=1;
			
			if (Min.Ge_Last==9 && Min.Ge_New==0) Min.Shi_Roll_Flag=1;
		
			if (Min.Shi_Last==5 && Min.Shi_New==0) Hour.Ge_Roll_Flag=1;
		
			if ((Hour.Ge_Last==9 && Hour.Ge_New==0) || (Hour.Ge_Last==3 && Hour.Ge_New==0))Hour.Shi_Roll_Flag=1;
			
			OLED_Roll();
		}
		Sec.Last=Sec.New;
		Min.Last=Min.New;
		Hour.Last=Hour.New;

}
