/********************************************************************************************************************
* LED驱动
********************************************************************************************************************/

#include "stm32f10x.h"                  // Device header


//-------------------------------------------------------------------------------------------------------------------
// 函数简介     LED初始化
// 备注信息     仅在"menu.c"的Peripheral_Init(){}中调用一次
//-------------------------------------------------------------------------------------------------------------------
void LED_Init(void)
{
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15|GPIO_Pin_12|GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_15);    // LED熄灭（高电平，LED是灌电流驱动）
	GPIO_ResetBits(GPIOB, GPIO_Pin_12);  // PB12拉低 → 使能电池ADC检测电路
	GPIO_SetBits(GPIOB, GPIO_Pin_13);    // PB13(CTL)拉高 → 维持PMOS电源自锁
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     LED开
//-------------------------------------------------------------------------------------------------------------------
void LED_ON(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_15);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     LED关
//-------------------------------------------------------------------------------------------------------------------
void LED_OFF(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_15);
}
