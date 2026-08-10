#include "stm32f10x.h"                  // Device header

#include "Delay.h"
#include "Timer.h"

#include "common_headfile.h"


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
	// (上电后)第一次读取按键
	uint8_t begin_flag = 1;
	
	while (1)
	{
		// 等待返回[菜单]和[设置]中的选择(逻辑位于"menu.c")
		clkflag1 = First_Page_Clock();
		if (begin_flag == 1)
		{
			clkflag1 = 0;
			begin_flag = 0;
		}
		// 跳转到[菜单]
		if (clkflag1 == 1){Menu();Key_Check(KEY_NAME_COMFIRM,KEY_LONG);}
		// 跳转到[设置]
		else if (clkflag1 == 2){SettingPage();Key_Check(KEY_NAME_COMFIRM,KEY_LONG);}
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
		if (Time_Count >= 10)
		{
			Time_Count = 0;
		}
		// 姿态解算与按键扫描错峰5ms：按键在TC≈9, 姿态在TC==5
		if (Time_Count == 5)
		{
			MPU6050_ANALYSIS_ENABLE = 1;			
		}
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
