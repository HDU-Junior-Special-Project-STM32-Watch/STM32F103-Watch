/*******************************************************************************
[菜单]表情包界面
*******************************************************************************/


#include "stm32f10x.h"
#include "common_headfile.h"

void Show_Emoji_UI(uint8_t tick)
{
	/*闭眼*/
	if (1 <= tick && tick <= 3)
	{
		tick -= 1;
		OLED_Clear();
		OLED_ShowImage( 30, 10+tick, 16, 16, Eyebrow[0]);//左眉毛
		OLED_ShowImage( 82, 10+tick, 16, 16, Eyebrow[1]);//右眉毛
		OLED_DrawEllipse( 40, 32, 6, 6-tick, 1);//左眼
		OLED_DrawEllipse( 88, 32, 6, 6-tick, 1);//右眼
		OLED_ShowImage( 54, 40, 20, 20, Mouth);
		OLED_Update();
	}
	
	/*睁眼*/
	if (4 <= tick && tick <= 6)
	{
		tick -= 4;
		OLED_Clear();
		OLED_ShowImage( 30, 12-tick, 16, 16, Eyebrow[0]);//左眉毛
		OLED_ShowImage( 82, 12-tick, 16, 16, Eyebrow[1]);//右眉毛
		OLED_DrawEllipse( 40, 32, 6, 4+tick, 1);//左眼
		OLED_DrawEllipse( 88, 32, 6, 4+tick, 1);//右眼
		OLED_ShowImage( 54, 40, 20, 20, Mouth);
		OLED_Update();
	}
}

int Emoji(void)
{
	// 时间参考值重置
	Time_Count1 = 0;
	Time_Count2 = 0;
	
	uint16_t Emoji_Tick = 1;
	
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
		
		if (Time_Count1 >= 100)
		{
			Time_Count1 = 0;
			
			Show_Emoji_UI(Emoji_Tick);
			Emoji_Tick ++;
			if (Emoji_Tick > 11){Emoji_Tick = 1;}
		}
	}
}
