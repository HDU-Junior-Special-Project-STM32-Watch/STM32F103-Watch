/*******************************************************************************
MPU6050零飘校准和姿态解算
*******************************************************************************/


#ifndef __MPU6050_ANALYSIS_PAGE_H__
#define __MPU6050_ANALYSIS_PAGE_H__

#include <stdint.h>

/**********************************************************/
/*[S] 全局变量 [S]----------------------------------------*/
/**********************************************************/

// 解算结果值
extern volatile float Yaw_Result;    	// 偏航角（Yaw）
extern volatile float Roll_Result;   	// 横滚角（Roll）
extern volatile float Pitch_Result;  	// 俯仰角（Pitch）

// IMU 通信+解析 使能标志位
extern volatile uint8_t IMU_D_and_A_En;

// 原始值
extern int16_t mpu6050_ax, mpu6050_ay, mpu6050_az;  // 加速度原始值
extern int16_t mpu6050_gx, mpu6050_gy, mpu6050_gz;  // 陀螺仪原始值
/**********************************************************/
/*----------------------------------------[E] 全局变量 [E]*/
/**********************************************************/


// 数据读取
void IMU_Update_Data(void);


/**********************************************************/
/*[S] 陀螺仪校准 [S]--------------------------------------*/
/**********************************************************/

// 用于存储陀螺仪校准的状态、参数和中间数据
typedef struct {
    // 校准状态
    uint8_t calib_state;  // 校准状态（0: 未校准, 1: 校准中, 2: 已校准）
    // 校准计数
    uint32_t calib_count;
    // 零点偏移量（整数，原始 ADC 单位）
    int16_t offset_x;   // X轴偏移
    int16_t offset_y;   // Y轴偏移
    int16_t offset_z;   // Z轴偏移  

    // 零飘数据采样累加（int32 足够容纳 500 次 int16 累加）
    int32_t sum_x;      // X轴数据累加和
    int32_t sum_y;      // Y轴数据累加和
    int32_t sum_z;      // Z轴数据累加和
} Gyro_Calib_StructDef;

// 枚举定义校准状态
typedef enum {
    GYRO_CALIB_STATE_IDLE    = 0,          // 未校准  
    GYRO_CALIB_STATE_RUNNING = 1,          // 校准中
    GYRO_CALIB_STATE_DONE    = 2           // 校准完
} Gyro_CalibState_t;

// 零飘校准校准需要的样本数
#define GYRO_CALIB_TARGET_SAMPLES    200  

// 对外声明实例
extern Gyro_Calib_StructDef gyro_cal;

void    IMU_Gyro_Calib_Start        (Gyro_Calib_StructDef *cal);
int     IMU_Gyro_Calib_Check        (Gyro_Calib_StructDef *cal);
void    IMU_Gyro_Apply              (Gyro_Calib_StructDef *cal, int16_t *gx, int16_t *gy, int16_t *gz);
/**********************************************************/
/*--------------------------------------[E] 陀螺仪校准 [E]*/
/**********************************************************/


// 姿态解算
void IMU_Update_Analysis(void);

#endif
