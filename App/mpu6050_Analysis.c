/*******************************************************************************
MPU6050零飘校准和姿态解算
*******************************************************************************/


#include "stm32f10x.h"                  // Device header
#include "common_headfile.h"
#include <math.h>

/**********************************************************/
/*[S] 全局变量 [S]----------------------------------------*/
/**********************************************************/

// 解算结果值
volatile float Yaw_Result = 0.0f;    // 偏航角（Yaw）
volatile float Roll_Result = 0.0f;   // 横滚角（Roll）
volatile float Pitch_Result = 0.0f;  // 俯仰角（Pitch）

// IMU 数据采集和分析使能标志位
volatile uint8_t IMU_D_and_A_En = 0;

// 原始值
int16_t mpu6050_ax = 0, mpu6050_ay = 0, mpu6050_az = 0; // 加速度原始值
int16_t mpu6050_gx = 0, mpu6050_gy = 0, mpu6050_gz = 0; // 陀螺仪原始值
/**********************************************************/
/*----------------------------------------[E] 全局变量 [E]*/
/**********************************************************/


/**********************************************************/
/*[S] 数据读取 [S]----------------------------------------*/
/**********************************************************/

// MPU6050 原始数据变量
// 加速度计原始值
// mpu6050_ax           mpu6050_ay          mpu6050_az
// 陀螺仪原始值
// mpu6050_gx          mpu6050_gy         mpu6050_gz
void IMU_Update_Data(void)
{
	MPU6050_GetData(&mpu6050_ax, &mpu6050_ay, &mpu6050_az,
                    &mpu6050_gx, &mpu6050_gy, &mpu6050_gz);
}
/**********************************************************/
/*----------------------------------------[E] 数据读取 [E]*/
/**********************************************************/



/**********************************************************/
/*[S] 陀螺仪校准 [S]--------------------------------------*/
/**********************************************************/

// 定义并初始化陀螺仪校准结构体
Gyro_Calib_StructDef gyro_cal = {
    .calib_state = GYRO_CALIB_STATE_IDLE, // 初始化为未校准

    .calib_count = 0,
    .offset_x = 0,
    .offset_y = 0,
    .offset_z = 0,
    .sum_x = 0,
    .sum_y = 0,
    .sum_z = 0,
};

//----------------------------------------------------------
// 函数简介     陀螺仪开始校准
// 使用示例     Gyro_Calib_Start(&gyro_cal);
//----------------------------------------------------------
void IMU_Gyro_Calib_Start(Gyro_Calib_StructDef *cal)
{
    cal->calib_state = GYRO_CALIB_STATE_RUNNING; // 设置状态为校准中

    cal->calib_count = 0;
    cal->sum_x = 0;
    cal->sum_y = 0;
    cal->sum_z = 0;
    cal->offset_x = 0;
    cal->offset_y = 0;
    cal->offset_z = 0;
}

//----------------------------------------------------------
// 函数简介     陀螺仪校准状态机
// 返回参数     校准状态：0-未校准，1-校准中，2-已校准
// 使用示例     Gyro_Calib_Check(&gyro_cal);
// 使用要求：
//   - 在静止状态下进行校准，确保设备不受外部运动干扰
//----------------------------------------------------------
int IMU_Gyro_Calib_Check(Gyro_Calib_StructDef *cal)
{
    if(cal->calib_state == GYRO_CALIB_STATE_DONE)
    {
        return 2; // 校准完
    }

    if(cal->calib_state == GYRO_CALIB_STATE_IDLE)
    {
        return 0; // 未校准
    }
    
    // 看来是 校准中 状态
    // 检查是否允许收集数据
    if(IMU_D_and_A_En)
    {
		// 数据读取
		// 读取内置在校准函数内部，调用IMU_Gyro_Calib_Check可直接查看状态并推进校准
		IMU_Update_Data();
		
        // 数据累加
        cal->sum_x += mpu6050_gx;
        cal->sum_y += mpu6050_gy;
        cal->sum_z += mpu6050_gz;
		
        cal->calib_count++;
		
        IMU_D_and_A_En = 0;
        
        // 样本数量达成目标
        if(cal->calib_count >= GYRO_CALIB_TARGET_SAMPLES)
        {
            // 计算陀螺仪整数偏移量（原始ADC单位）
            cal->offset_x = cal->sum_x / GYRO_CALIB_TARGET_SAMPLES;
            cal->offset_y = cal->sum_y / GYRO_CALIB_TARGET_SAMPLES;
            cal->offset_z = cal->sum_z / GYRO_CALIB_TARGET_SAMPLES;
            
            // 更新陀螺仪校准状态为完成
            cal->calib_state = GYRO_CALIB_STATE_DONE;
        }
    }
    return 1; // 校准中
}

//----------------------------------------------------------
// 函数简介     应用陀螺仪校准参数
// 参数说明     cal             陀螺仪校准结构体指针
// 参数说明     gx              用于存储校准后X轴数据的指针
// 参数说明     gy              用于存储校准后Y轴数据的指针
// 参数说明     gz              用于存储校准后Z轴数据的指针
// 使用示例     float gx, gy, gz;
// 使用示例     IMU_Gyro_Apply(&gyro_cal, &gx, &gy, &gz);
// 备注信息     函数内部直接使用全局变量 mpu6050_gx/gy/gz 获取原始数据
//              校准完成后，将校准结果存储到传入的指针中
//              未校准时，应用默认的校准参数
//----------------------------------------------------------
void IMU_Gyro_Apply(Gyro_Calib_StructDef *cal, int16_t *gx, int16_t *gy, int16_t *gz)
{
    // 如果校准完成，应用校准参数（仍然保留为整数）
    if (cal->calib_state == GYRO_CALIB_STATE_DONE)
    {
        *gx = mpu6050_gx - cal->offset_x;
        *gy = mpu6050_gy - cal->offset_y;
        *gz = mpu6050_gz - cal->offset_z;
    }
    // 未校准时，应用默认偏移
    else
    {
        *gx = mpu6050_gx;
        *gy = mpu6050_gy;
        *gz = mpu6050_gz;
    }

    // "死区"
    if (-7 < *gx && *gx < 7){*gx = 0;}
    if (-7 < *gy && *gy < 7){*gy = 0;}
    if (-7 < *gz && *gz < 7){*gz = 0;}
}
/**********************************************************/
/*--------------------------------------[E] 陀螺仪校准 [E]*/
/**********************************************************/


/**********************************************************/
/*[S] 姿态解算 [S]----------------------------------------*/
/**********************************************************/
float a=0.9;//互补滤波器系数
float Delta_t=0.01;//采样周期
#define RAD_TO_DEG (180.0f / 3.1415927f)	// 弧度转角度系数，预计算避免运行时除法

void IMU_Update_Analysis(void)
{
	// 读取内置在解算函数内部，与校准函数类似
	IMU_Update_Data();
	
    // 加速度直接在后续调用原始值
    // 陀螺仪应用校准数据
	int16_t gx, gy, gz;
	IMU_Gyro_Apply(&gyro_cal, &gx, &gy, &gz);
	
	//通过陀螺仪解算欧拉角
	float roll_g = Roll_Result + (float)gx * Delta_t;
	float pitch_g = Pitch_Result + (float)gy * Delta_t;
	float yaw_g = Yaw_Result + (float)gz * Delta_t;
	
	//通过加速度计解算欧拉角
	float pitch_a = atan2(-mpu6050_ax, mpu6050_az) * RAD_TO_DEG;
	float roll_a  = atan2( mpu6050_ay, mpu6050_az) * RAD_TO_DEG;
	
	//通过互补滤波器进行数据融合
	Roll_Result = a * roll_g + (1-a) * roll_a;
	Pitch_Result = a * pitch_g + (1-a) * pitch_a;
	Yaw_Result = a * yaw_g;
	
	IMU_D_and_A_En = 0;
}
/**********************************************************/
/*----------------------------------------[E] 姿态解算 [E]*/
/**********************************************************/
