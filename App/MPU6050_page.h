/*******************************************************************************
[菜单]MPU6050界面
*******************************************************************************/


#ifndef __MYMPU6050_PAGE_H
#define __MYMPU6050_PAGE_H

#include <stdint.h>

// MPU6050解算使能标志位
extern uint8_t MPU6050_ANALYSIS_ENABLE;

void 	quat2euler		(float q0,float q1,float q2,float q3,
						volatile float* phi, volatile float* theta, volatile float* psi);

void	 MPU6050_Calculation(void);
int 	MPU6050			(void);

#endif
