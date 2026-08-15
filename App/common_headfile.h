/*******************************************************************************
公共头文件
*******************************************************************************/


#ifndef __COMMON_HEADFILE_H__
#define __COMMON_HEADFILE_H__

#include "stm32f10x.h"

/* 应用层 */
#include "menu.h"
#include "Roll_Clock.h"
#include "SetTime_page.h"
#include "SetBrightness_page.h"
#include "StopWatch_page.h"
#include "MPU6050_page.h"
#include "LEDFlashlight_page.h"

/* 驱动层 */
#include "OLED.h"
#include "LED.h"
#include "Key.h"
#include "MPU6050.h"
#include "AD.h"

/* 算法库 */
#include "mpu6050_Analysis.h"

#endif
