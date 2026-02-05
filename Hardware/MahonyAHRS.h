//=====================================================================================================
// MahonyAHRS.h
//=====================================================================================================
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
//=====================================================================================================
#ifndef MahonyAHRS_h
#define MahonyAHRS_h

//----------------------------------------------------------------------------------------------------
// Variable declaration
// 变量声明
//----------------------------------------------------------------------------------------------------

extern volatile float twoKp;			// 2 * proportional gain (Kp)
										// 2倍的比例增益（Kp）
extern volatile float twoKi;			// 2 * integral gain (Ki)
										// 2倍的积分增益（Ki）
extern volatile float q0, q1, q2, q3;	// quaternion of sensor frame relative to auxiliary frame
										// 传感器坐标系相对于辅助坐标系的四元数

//---------------------------------------------------------------------------------------------------
// Function declarations
// 函数声明
//---------------------------------------------------------------------------------------------------

// 融合陀螺仪、加速度计、磁力计的AHRS姿态更新函数
// 输入参数：
// gx/gy/gz - 陀螺仪角速度（rad/s）
// ax/ay/az - 加速度计测量值（m/s²）
// mx/my/mz - 磁力计测量值（uT）
void MahonyAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);

// 仅融合陀螺仪、加速度计的IMU姿态更新函数（无磁力计）
// 输入参数：
// gx/gy/gz - 陀螺仪角速度（rad/s）
// ax/ay/az - 加速度计测量值（m/s²）
void MahonyAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, float az);

#endif
//=====================================================================================================
// End of file
// 文件结束
//=====================================================================================================
