#include "stm32f10x.h"                  // Device header

#include "Delay.h"
#include "Timer.h"

#include "menu.h"
#include "OLED.h"
#include "param_config.h"


int main(void)
{
	/* OLED初始化*/
	OLED_Init();
	OLED_Clear();
	
	/* 外设初始化*/
	Peripheral_Init();
	
	/* 1ms定时器定时中断初始化*/
	Timer_Init();	
	
	// 首页选项标志位
	uint8_t clkflag1 = 0;
	
	while (1)
	{
		// 等待返回[菜单]和[设置]中的选择(逻辑位于"menu.c")
		clkflag1 = First_Page_Clock();
		// 跳转到[菜单]
		if (clkflag1 == 1){Menu();}
		// 跳转到[设置]
		else if (clkflag1 == 2){SettingPage();}
	}
}

uint16_t Time_Count = 0;

/* TIM2定时器1ms定时中断*/
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		Key_Tick();
		StopWatch_Tick();
		
		Time_Count ++;
		if (Time_Count >= 2)
		{
			Time_Count = 0;
			MPU6050_ANALYSIS_ENABLE = 1;			
		}
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
