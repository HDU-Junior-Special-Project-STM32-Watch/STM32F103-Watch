/********************************************************************************************************************
* 亮度设置界面
********************************************************************************************************************/

#include "stm32f10x.h"                  // Device header

#include "MyRTC.h"

#include "OLED.h"
#include "Key.h"


uint8_t Brightness_Level = 4;
/*******************************************************************************************************************/
/*[S] 界面样式 [S]-------------------------------------------------------------------------------------------------*/
/*******************************************************************************************************************/

void Show_SetBrightness_UI(void)
{
	OLED_ShowImage(0, 0, 16, 16, Return);
	OLED_Printf(0, 16, OLED_8X16, "亮度:");
	OLED_ShowImage(48, 16, 16, 16, Brightness_Image);
	OLED_ShowImage(64, 16, 16, 16, Brightness_Image);
	OLED_ShowImage(80, 16, 16, 16, Brightness_Image);
	OLED_ShowImage(96, 16, 16, 16, Brightness_Image);
	OLED_ShowImage(112, 16, 16, 16, Brightness_Image);
	OLED_ReverseArea(48, 16, 16 * Brightness_Level, 16);
}
/*******************************************************************************************************************/
/*[E] 界面样式 [E]-------------------------------------------------------------------------------------------------*/
/*******************************************************************************************************************/


/*******************************************************************************************************************/
/*[S] 亮度配置 [S]-------------------------------------------------------------------------------------------------*/
/*******************************************************************************************************************/

// 亮度档位映射表
static const uint8_t OLED_Brightness_contrastTable[6] = {
    0x00,    	// 0 备用/极暗，不在正常循环内
    0x32,   	// 1/5  ≈20%
    0x64,  		// 2/5  ≈40%
    0x96,  		// 3/5  ≈60%
    0xC8,  		// 4/5  ≈80%
    0xFA   		// 5/5  ≈100%
};

// 写对比度寄存器
void Write_OLED_Brightness_CMD(uint8_t Brightness)
{
	// 越界判定
	if (Brightness < 1) Brightness = 1;
    if (Brightness > 5) Brightness = 5;
	
	OLED_WriteCommand(0x81);                 // 对比度寄存器
    OLED_WriteCommand(OLED_Brightness_contrastTable[Brightness]);
}

/* 交互界面*/
int Set_OLED_Brightness(void)
{
	while(1)
	{
		// 上键，亮度减
		if (Key_Check(KEY_NAME_UP,KEY_SINGLE))
		{
			Brightness_Level --;
			if (Brightness_Level < 1)Brightness_Level = 1;
		}
		// 下键，亮度加
		else if (Key_Check(KEY_NAME_DOWN,KEY_SINGLE))
		{
			Brightness_Level ++;
			if (Brightness_Level > 5)Brightness_Level = 5;
		}
		// 确认键，退出
		else if (Key_Check(KEY_NAME_COMFIRM,KEY_SINGLE))
		{
			return 0;
		}
		
		// 写入对比度（亮度）
		Write_OLED_Brightness_CMD(Brightness_Level);
		Show_SetBrightness_UI();
		OLED_Update();
	}
}
/*******************************************************************************************************************/
/*[E] 亮度配置 [E]-------------------------------------------------------------------------------------------------*/
/*******************************************************************************************************************/


/*******************************************************************************************************************/
/*[S] 交互界面 [S]-------------------------------------------------------------------------------------------------*/
/*******************************************************************************************************************/

// 日期时间设置界面选项标志位
uint8_t set_brightness_flag = 1;

uint8_t SetBrightness(void)
{
	while(1)
	{
		// 存储确认键被按下时set_brightness_flag的值的临时变量，默认为无效值0
		uint8_t set_brightness_flag_temp = 0;
		
		/* 按键处理*/
		if (Key_Check(KEY_NAME_UP,KEY_SINGLE))// 上键
		{
			set_brightness_flag --;
			if (set_brightness_flag <= 0) set_brightness_flag = 2;
		}		
		else if (Key_Check(KEY_NAME_DOWN,KEY_SINGLE))// 下键
		{
			set_brightness_flag ++;
			if (set_brightness_flag >= 3) set_brightness_flag = 1;
		}		
		else if (Key_Check(KEY_NAME_COMFIRM,KEY_SINGLE))// 确认键
		{
			set_brightness_flag_temp = set_brightness_flag;
		}
		
		/* 功能跳转*/
		// 返回上一级菜单
		if (set_brightness_flag_temp == 1){OLED_Clear();return 0;}
		// 进入亮度设置
		else if (set_brightness_flag_temp == 2){Set_OLED_Brightness();}
			
		/* 显示更新*/
		switch(set_brightness_flag)
		{
			// 光标在第一行"回车"键
			case 1:
			{
				OLED_Clear();
				Show_SetBrightness_UI();
				OLED_ReverseArea(0, 0, 16, 16);
				OLED_Update();
				
				break;
			}
			// 光标在第二行"亮度"选项
			case 2:
			{
				OLED_Clear();
				Show_SetBrightness_UI();
				OLED_ReverseArea(0, 16, 32, 16);
				OLED_Update();
				
				break;
			}
		}
	}
}
/*******************************************************************************************************************/
/*[E] 交互界面 [E]-------------------------------------------------------------------------------------------------*/
/*******************************************************************************************************************/
