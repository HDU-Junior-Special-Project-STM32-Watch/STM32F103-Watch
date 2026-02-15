#ifndef __MENU_H
#define __MENU_H

#include "Key.h"


void 	Peripheral_Init		(void);
void 	Show_Clock_UI		(uint8_t clkflag);
int 	First_Page_Clock	(void);
int 	SettingPage			(void);
int 	Menu				(void);

#endif
