/*******************************************************************************
[首页]+[菜单]+[设置]
*******************************************************************************/


#ifndef __MENU_H__
#define __MENU_H__

#include <stdint.h>

void 	Peripheral_Init		(void);
void 	Show_Clock_UI		(uint8_t clkflag);
int 	First_Page_Clock	(void);
int 	SettingPage			(void);
int 	Menu				(void);

#endif
