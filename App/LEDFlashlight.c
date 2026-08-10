/*******************************************************************************
[菜单]LED手电筒界面
*******************************************************************************/

#include "stm32f10x.h"                  // Device header

#include "common_headfile.h"


// 控制手电筒开关的函数
int LED_Flashlight(void)
{
	/* 读取当前LED状态：低电平为亮 */
	uint8_t led_on = (GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_15) == Bit_RESET);
	uint8_t refresh = 1;	// 首帧刷新

	while(1)
	{
		/* 按键：任意方向键翻转LED */
		if (Key_Check(KEY_NAME_UP,   KEY_SINGLE) ||
		    Key_Check(KEY_NAME_DOWN, KEY_SINGLE))
		{
			led_on = !led_on;
			if (led_on) LED_ON();
			else        LED_OFF();
			refresh = 1;
		}
		/* 按键：确认键退出 */
		else if (Key_Check(KEY_NAME_COMFIRM, KEY_SINGLE))
		{
			OLED_Clear();
			OLED_Update();
			return 0;
		}
		
		/* 仅在状态变化时刷新显示 */
		if (refresh)
		{
			OLED_Clear();
			OLED_ShowImage(0, 0, 16, 16, Return);
			OLED_ReverseArea(0, 0, 16, 16);				// 确认键=返回
			if (led_on)
				OLED_ShowString(52, 20, "ON",  OLED_12X24);
			else
				OLED_ShowString(46, 20, "OFF", OLED_12X24);
			OLED_Update();
			refresh = 0;
		}
	}
}
