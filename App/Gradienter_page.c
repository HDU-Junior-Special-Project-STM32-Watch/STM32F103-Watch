/*******************************************************************************
[菜单]水平仪界面
*******************************************************************************/


#include "stm32f10x.h"
#include "common_headfile.h"

int Gradienter(void)
{
	uint8_t refresh = 1;	// UI刷新标志位
	float Roll = 0.0f, Pitch = 0.0f;
	
	while (1)
	{
		/* 按键处理 */
		Key_Check(KEY_NAME_UP,KEY_SINGLE);// 上键
		Key_Check(KEY_NAME_DOWN,KEY_SINGLE);// 下键
		if (Key_Check(KEY_NAME_COMFIRM,KEY_SINGLE))// 确认键
		{
			// 清屏OLED，准备跳转
			OLED_Clear();
			OLED_Update();
			
			return 0;
		}
		
		/* 姿态解算*/
		if (IMU_D_and_A_En == 1)
		{
			IMU_Update_Analysis();
			refresh=1;
		}
		
		/* 显示更新 */
		if (refresh)
		{
			// 一阶低通滤波
			Roll = Roll * 0.4f + Roll_Result * 0.6f;
			Pitch = Pitch * 0.4f + Pitch_Result * 0.6f;
			
			OLED_Clear();
			// X64Y32,R30,不填充
			OLED_DrawCircle( 64, 32, 30,  0);
			// X64Y32,R4,填充
			OLED_DrawCircle( 64 - Roll * 0.5f, 32 + Pitch * 0.5f,  4,  1);
			OLED_Update();
			refresh = 0;
		}
	}
}
