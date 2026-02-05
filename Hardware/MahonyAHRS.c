//===================================================================================================================
// MahonyAHRS.c
//===================================================================================================================
//
// Madgwick's implementation of Mayhony's AHRS algorithm.
// Madgwick对Mahony的AHRS（姿态与航向参考系统）算法的实现版本
// See: http://www.x-io.co.uk/node/8#open_source_ahrs_and_imu_algorithms
// 参考链接：http://www.x-io.co.uk/node/8#open_source_ahrs_and_imu_algorithms
//
// Date			Author			Notes
// 日期			作者			备注
// 29/09/2011	SOH Madgwick    Initial release
// 2011年9月29日	SOH Madgwick    初始版本发布
// 02/10/2011	SOH Madgwick	Optimised for reduced CPU load
// 2011年10月2日	SOH Madgwick	优化以降低CPU负载
//
//===================================================================================================================

//-------------------------------------------------------------------------------------------------------------------
// Header files
// 头文件包含
//-------------------------------------------------------------------------------------------------------------------

#include "MahonyAHRS.h"
#include <math.h>

//-------------------------------------------------------------------------------------------------------------------
// Definitions
// 宏定义
//-------------------------------------------------------------------------------------------------------------------

#define sampleFreq	500.0f			// sample frequency in Hz
									// 采样频率，单位为赫兹(Hz)
#define twoKpDef	(2.0f * 50.0f)	// 2 * proportional gain
									// 2倍的比例增益（Kp）
#define twoKiDef	(2.0f * 1.0f)	// 2 * integral gain
									// 2倍的积分增益（Ki）

//-------------------------------------------------------------------------------------------------------------------
// Variable definitions
// 变量定义
//-------------------------------------------------------------------------------------------------------------------

volatile float twoKp = twoKpDef;											// 2 * proportional gain (Kp)
																			// 2倍的比例增益（Kp）
volatile float twoKi = twoKiDef;											// 2 * integral gain (Ki)
																			// 2倍的积分增益（Ki）
volatile float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;					// quaternion of sensor frame relative to auxiliary frame
																			// 传感器坐标系相对于辅助坐标系的四元数
volatile float integralFBx = 0.0f,  integralFBy = 0.0f, integralFBz = 0.0f;	// integral error terms scaled by Ki
																			// 由Ki缩放后的积分误差项

//-------------------------------------------------------------------------------------------------------------------
// Function declarations
// 函数声明
//-------------------------------------------------------------------------------------------------------------------

float invSqrt(float x);			// 快速逆平方根函数声明

//===================================================================================================================
// Functions
// 函数定义
//===================================================================================================================

//-------------------------------------------------------------------------------------------------------------------
// AHRS algorithm update
// AHRS算法更新函数
//-------------------------------------------------------------------------------------------------------------------

void MahonyAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz) {
	float recipNorm;				// 归一化系数（倒数形式）
    float q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;  // 四元数乘积临时变量  
	float hx, hy, bx, bz;			// 地磁向量相关临时变量
	float halfvx, halfvy, halfvz, halfwx, halfwy, halfwz;	// 重力/地磁估计方向临时变量
	float halfex, halfey, halfez;	// 误差项临时变量（半值形式）
	float qa, qb, qc;				// 四元数更新临时变量

	// Use IMU algorithm if magnetometer measurement invalid (avoids NaN in magnetometer normalisation)
	// 若磁力计测量值无效（全0），则使用IMU算法（避免磁力计归一化时出现非数值(NaN)）
	if((mx == 0.0f) && (my == 0.0f) && (mz == 0.0f)) {
		MahonyAHRSupdateIMU(gx, gy, gz, ax, ay, az);
		return;
	}

	// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
	// 仅当加速度计测量值有效时计算反馈（避免加速度计归一化时出现非数值(NaN)）
	if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

		// Normalise accelerometer measurement
		// 归一化加速度计测量值
		recipNorm = invSqrt(ax * ax + ay * ay + az * az);
		ax *= recipNorm;
		ay *= recipNorm;
		az *= recipNorm;     

		// Normalise magnetometer measurement
		// 归一化磁力计测量值
		recipNorm = invSqrt(mx * mx + my * my + mz * mz);
		mx *= recipNorm;
		my *= recipNorm;
		mz *= recipNorm;   

        // Auxiliary variables to avoid repeated arithmetic
        // 辅助变量，避免重复计算
        q0q0 = q0 * q0;
        q0q1 = q0 * q1;
        q0q2 = q0 * q2;
        q0q3 = q0 * q3;
        q1q1 = q1 * q1;
        q1q2 = q1 * q2;
        q1q3 = q1 * q3;
        q2q2 = q2 * q2;
        q2q3 = q2 * q3;
        q3q3 = q3 * q3;   

        // Reference direction of Earth's magnetic field
        // 地球磁场的参考方向（转换到传感器坐标系）
        hx = 2.0f * (mx * (0.5f - q2q2 - q3q3) + my * (q1q2 - q0q3) + mz * (q1q3 + q0q2));
        hy = 2.0f * (mx * (q1q2 + q0q3) + my * (0.5f - q1q1 - q3q3) + mz * (q2q3 - q0q1));
        bx = sqrt(hx * hx + hy * hy);
        bz = 2.0f * (mx * (q1q3 - q0q2) + my * (q2q3 + q0q1) + mz * (0.5f - q1q1 - q2q2));

		// Estimated direction of gravity and magnetic field
		// 估计的重力方向和磁场方向
		halfvx = q1q3 - q0q2;
		halfvy = q0q1 + q2q3;
		halfvz = q0q0 - 0.5f + q3q3;
        halfwx = bx * (0.5f - q2q2 - q3q3) + bz * (q1q3 - q0q2);
        halfwy = bx * (q1q2 - q0q3) + bz * (q0q1 + q2q3);
        halfwz = bx * (q0q2 + q1q3) + bz * (0.5f - q1q1 - q2q2);  
	
		// Error is sum of cross product between estimated direction and measured direction of field vectors
		// 误差为场向量估计方向与测量方向的叉乘之和
		halfex = (ay * halfvz - az * halfvy) + (my * halfwz - mz * halfwy);
		halfey = (az * halfvx - ax * halfvz) + (mz * halfwx - mx * halfwz);
		halfez = (ax * halfvy - ay * halfvx) + (mx * halfwy - my * halfwx);

		// Compute and apply integral feedback if enabled
		// 若积分反馈使能，则计算并应用积分反馈
		if(twoKi > 0.0f) {
			integralFBx += twoKi * halfex * (1.0f / sampleFreq);	// integral error scaled by Ki
																	// 由Ki缩放的积分误差项
			integralFBy += twoKi * halfey * (1.0f / sampleFreq);
			integralFBz += twoKi * halfez * (1.0f / sampleFreq);
			gx += integralFBx;	// apply integral feedback
								// 应用积分反馈
			gy += integralFBy;
			gz += integralFBz;
		}
		else {
			integralFBx = 0.0f;	// prevent integral windup
								// 防止积分饱和
			integralFBy = 0.0f;
			integralFBz = 0.0f;
		}

		// Apply proportional feedback
		// 应用比例反馈
		gx += twoKp * halfex;
		gy += twoKp * halfey;
		gz += twoKp * halfez;
	}
	
	// Integrate rate of change of quaternion
	// 积分更新四元数的变化率
	gx *= (0.5f * (1.0f / sampleFreq));		// pre-multiply common factors
											// 预乘公共因子，减少重复计算
	gy *= (0.5f * (1.0f / sampleFreq));
	gz *= (0.5f * (1.0f / sampleFreq));
	qa = q0;
	qb = q1;
	qc = q2;
	q0 += (-qb * gx - qc * gy - q3 * gz);
	q1 += (qa * gx + qc * gz - q3 * gy);
	q2 += (qa * gy - qb * gz + q3 * gx);
	q3 += (qa * gz + qb * gy - qc * gx); 
	
	// Normalise quaternion
	// 归一化四元数（保证为单位四元数）
	recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q0 *= recipNorm;
	q1 *= recipNorm;
	q2 *= recipNorm;
	q3 *= recipNorm;
}

//-------------------------------------------------------------------------------------------------------------------
// IMU algorithm update
// IMU算法更新函数
//-------------------------------------------------------------------------------------------------------------------

void MahonyAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, float az) {
	float recipNorm;				// 归一化系数（倒数形式）
	float halfvx, halfvy, halfvz;	// 重力估计方向临时变量
	float halfex, halfey, halfez;	// 误差项临时变量（半值形式）
	float qa, qb, qc;				// 四元数更新临时变量

	// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
	// 仅当加速度计测量值有效时计算反馈（避免加速度计归一化时出现非数值(NaN)）
	if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

		// Normalise accelerometer measurement
		// 归一化加速度计测量值
		recipNorm = invSqrt(ax * ax + ay * ay + az * az);
		ax *= recipNorm;
		ay *= recipNorm;
		az *= recipNorm;        

		// Estimated direction of gravity and vector perpendicular to magnetic flux
		// 估计的重力方向（与磁通量垂直的向量）
		halfvx = q1 * q3 - q0 * q2;
		halfvy = q0 * q1 + q2 * q3;
		halfvz = q0 * q0 - 0.5f + q3 * q3;
	
		// Error is sum of cross product between estimated and measured direction of gravity
		// 误差为重力估计方向与测量方向的叉乘之和
		halfex = (ay * halfvz - az * halfvy);
		halfey = (az * halfvx - ax * halfvz);
		halfez = (ax * halfvy - ay * halfvx);

		// Compute and apply integral feedback if enabled
		// 若积分反馈使能，则计算并应用积分反馈
		if(twoKi > 0.0f) {
			integralFBx += twoKi * halfex * (1.0f / sampleFreq);	// integral error scaled by Ki
																	// 由Ki缩放的积分误差项
			integralFBy += twoKi * halfey * (1.0f / sampleFreq);
			integralFBz += twoKi * halfez * (1.0f / sampleFreq);
			gx += integralFBx;	// apply integral feedback
								// 应用积分反馈
			gy += integralFBy;
			gz += integralFBz;
		}
		else {
			integralFBx = 0.0f;	// prevent integral windup
								// 防止积分饱和
			integralFBy = 0.0f;
			integralFBz = 0.0f;
		}

		// Apply proportional feedback
		// 应用比例反馈
		gx += twoKp * halfex;
		gy += twoKp * halfey;
		gz += twoKp * halfez;
	}
	
	// Integrate rate of change of quaternion
	// 积分更新四元数的变化率
	gx *= (0.5f * (1.0f / sampleFreq));		// pre-multiply common factors
											// 预乘公共因子，减少重复计算
	gy *= (0.5f * (1.0f / sampleFreq));
	gz *= (0.5f * (1.0f / sampleFreq));
	qa = q0;
	qb = q1;
	qc = q2;
	q0 += (-qb * gx - qc * gy - q3 * gz);
	q1 += (qa * gx + qc * gz - q3 * gy);
	q2 += (qa * gy - qb * gz + q3 * gx);
	q3 += (qa * gz + qb * gy - qc * gx); 
	
	// Normalise quaternion
	// 归一化四元数（保证为单位四元数）
	recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q0 *= recipNorm;
	q1 *= recipNorm;
	q2 *= recipNorm;
	q3 *= recipNorm;
}

//-------------------------------------------------------------------------------------------------------------------
// Fast inverse square-root
// 快速逆平方根函数
// See: http://en.wikipedia.org/wiki/Fast_inverse_square_root
// 参考链接：http://en.wikipedia.org/wiki/Fast_inverse_square_root
//-------------------------------------------------------------------------------------------------------------------

float invSqrt(float x) {
	float halfx = 0.5f * x;		// x的一半
	float y = x;				// 临时变量y，初始化为x
	long i = *(long*)&y;		// 将float类型的y强制转换为long类型，用于位操作
	i = 0x5f3759df - (i>>1);	// 魔数运算，快速逼近逆平方根结果
	y = *(float*)&i;			// 将long类型的结果转换回float类型
	y = y * (1.5f - (halfx * y * y));	// 牛顿迭代法一次优化，提升精度
	return y;
}
//===================================================================================================================
// END OF CODE
// 代码结束
//===================================================================================================================
