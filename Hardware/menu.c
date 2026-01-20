#include "stm32f10x.h"                  // Device header

#include "MyRTC.h"

#include "OLED.h"
#include "LED.h"
#include "Key.h"
#include "SetTime.h"					//日期时间设置模块

/*--------------------[S] 外设初始化 [S]--------------------*/

void Peripheral_Init(void)
{
	MyRTC_Init();
	LED_Init();
	Key_Init();
}
/*--------------------[E] 外设初始化 [E]--------------------*/


/*--------------------[S] 首页时钟 [S]--------------------*/

void Show_Clock_UI(void)
{
	//读取RTC时间
	MyRTC_ReadTime();
	
	//显示年月日(屏幕左上角)
	OLED_Printf(0, 0, OLED_6X8, "%d-%d-%d", MyRTC_Time[0], MyRTC_Time[1], MyRTC_Time[2]);
	//显示时分秒(屏幕中央)
	OLED_Printf(16, 16, OLED_12X24 , "%02d:%02d:%02d", MyRTC_Time[3], MyRTC_Time[4], MyRTC_Time[5]);
	//显示"菜单"(屏幕左下角)16X16
	OLED_ShowString(0, 48, "菜单", OLED_8X16);
	//显示"设置"(屏幕右下角)16X16
	OLED_ShowString(96, 48, "设置", OLED_8X16);
}

//首页选项标志位
//菜单		1
//设置		2
uint8_t clkflag = 1;

uint8_t First_Page_Clock(void)
{
	while(1)
	{
		//上键
		if (Key_Check(KEY_NAME_UP,KEY_SINGLE))
		{
			clkflag --;
			if (clkflag <= 0) clkflag = 2;
		}
		//下键
		else if (Key_Check(KEY_NAME_DOWN,KEY_SINGLE))
		{
			clkflag ++;
			if (clkflag >= 3) clkflag = 1;
		}
		//确认键
		else if (Key_Check(KEY_NAME_COMFIRM,KEY_SINGLE))
		{
			//清屏OLED，准备跳转
			OLED_Clear();
			OLED_Update();
			
			return clkflag;
		}
		
		switch(clkflag)
		{
			//光标在"菜单"
			case 1:
			{
				Show_Clock_UI();
				OLED_ReverseArea(0, 48, 32, 16);
				OLED_Update();
				
				break;
			}
			//光标在"设置"
			case 2:
			{
				Show_Clock_UI();
				OLED_ReverseArea(96, 48, 32, 16);
				OLED_Update();
				
				break;
			}
			
			
		}
	}
}
/*--------------------[E] 首页时钟 [E]--------------------*/


/*--------------------[S] 设置界面 [S]--------------------*/

//设置界面显示
void Show_SettingPage_UI(void)
{
	OLED_ShowImage(0, 0, 16, 16, Return);
	OLED_ShowString(0, 16, "日期时间设置", OLED_8X16);
//	OLED_ShowString(0, 32, "亮度设置", OLED_8X16);
}

//设置界面选项标志位
uint8_t setflag = 1;
uint8_t SettingPage(void)
{
	while(1)
	{
		//存储确认键被按下时setflag的值的临时变量，默认为无效值0
		uint8_t setflag_temp = 0;
		
		//上键
		if (Key_Check(KEY_NAME_UP,KEY_SINGLE))
		{
			setflag --;
			if (setflag <= 0) setflag = 2;
//			if (setflag <= 0) setflag = 3;
		}
		//下键
		else if (Key_Check(KEY_NAME_DOWN,KEY_SINGLE))
		{
			setflag ++;
			if (setflag >= 3) setflag = 1;
//			if (setflag >= 4) setflag = 1;
		}
		//确认键
		else if (Key_Check(KEY_NAME_COMFIRM,KEY_SINGLE))
		{
			//清屏OLED，准备跳转
			OLED_Clear();
			OLED_Update();
			
			setflag_temp = setflag;
		}
		
		//返回上一级菜单
		if (setflag_temp == 1){return 0;}
		//进入日期时间设置界面
		else if (setflag_temp == 2){SetTime();}
		
		switch(setflag)
		{
			//光标在第一行"回车"键
			case 1:
			{
				Show_SettingPage_UI();
				OLED_ReverseArea(0, 0, 16, 16);
				OLED_Update();
				
				break;
			}
			//光标在第二行"日期时间设置"选项
			case 2:
			{
				Show_SettingPage_UI();
				OLED_ReverseArea(0, 16, 96, 16);
				OLED_Update();
				
				break;
			}
			//光标在第三行"亮度设置"选项
//			case 3:
//			{
//				Show_SettingPage_UI();
//				OLED_ReverseArea(0, 32, 64, 16);
//				OLED_Update();
//				
//				break;
//			}
			
			
		}
	}
}
/*--------------------[E] 设置界面 [E]--------------------*/
