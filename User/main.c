////#include "function.h"
//#include "function.h"
//
//
//int main(void)
//{
//    uint8_t mode = 0; // 当前模式 (0: 未选择, 1: 第一题, 2: 第二题, 3: 第三题)
//
//    // 各个模块初始化
//    LED_Init();
//    OLED_Init();
//    Infrared_Init();
//    Motor_Init();
//    Key_Init();
//    Buzzer_Init();
//    MPU_Init();
//		
//    OLED_NAME();
//
//    while (1)
//    {
//        // 按下KEY_WK (模式选择)
//        if (GetKeyState(GPIO_Pin_0, GPIOA))
//        {
//            mode++;
//            if (mode > 3) mode = 1; // 循环切换模式 (1, 2, 3)
//
//            // 根据模式显示状态
//            if (mode == 1)
//            {
//                OLED_ShowString(1, 1, "Mode 1       ");
//                OLED_ShowString(2, 1, "             ");
//                OLED_ShowString(3, 1, "             ");
//                OLED_ShowString(4, 1, "             ");
//            }
//            else if (mode == 2)
//            {
//                OLED_ShowString(1, 1, "Mode 2       ");
//                OLED_ShowString(2, 1, "             ");
//                OLED_ShowString(3, 1, "             ");
//                OLED_ShowString(4, 1, "             ");
//            }
//            else if (mode == 3)
//            {
//                OLED_ShowString(1, 1, "Mode 3       ");
//                OLED_ShowString(2, 1, "             ");
//                OLED_ShowString(3, 1, "             ");
//                OLED_ShowString(4, 1, "             ");
//            }
//
//            Motor_Stop(); // 切换模式时停止小车
//            one = 0; // 重置黑线检测标志
//        }
//
//        if (mode == 1) // 第一题
//        {
//            made1();
//        }
//        else if (mode == 2) // 第二题
//        {
//            made2();
//        }
//        else if (mode == 3) // 第三题
//        {
//						made3();
//        }
//    }
//}
#include "stm32f10x.h"                  // 设备头文件
#include "Delay.h"
#include "OLED.h"
#include "Motor.h"
#include "Infrared.h"
#include "Key.h"
#include "buzzer.h"
#include "LED.h"
#include <math.h>
#include "mpu6050.h"
#include "mpuiic.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "sys.h"
#include "usart.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"

#include "pid.h"

// 定义PID控制器
PID turnPID;  // 定义PID控制器结构体
float targetAngle = 90.0;  // 目标角度90度
float currentAngle = 0.0;  // 当前角度
float angleError = 0.0;  // 当前角度与目标角度的误差

// PID参数
float Kp = 5.0, Ki = 0.1, Kd = 2.0;  // 调整PID参数，降低Kp和Kd
float maxI = 50.0, maxOutput = 50.0;  // PID控制器的积分限幅和输出限幅

// 初始化PID
void PID_Init_Car(void) {
    PID_Init(&turnPID, Kp, Ki, Kd, maxI, maxOutput);
}

// 获取当前角度（通过MPU6050）
void Get_Current_Angle(void) {
    short gx, gy, gz;
    // 获取陀螺仪的角度
    MPU_Get_Gyroscope(&gx, &gy, &gz);
    // 假设gz表示当前Z轴的角度变化速率（可以根据实际需求调整）
    currentAngle += gz * 0.01;  // 假设采样周期为10ms，单位为度
}

// 控制小车转弯90度
void Turn_90_Degrees(void) {
    // 获取当前角度
    Get_Current_Angle();
    
    // 计算角度误差
    angleError = targetAngle - currentAngle;

    // 进行PID计算
    PID_Calc(&turnPID, targetAngle, currentAngle);
    
    //// 根据PID输出控制电机
    //if (angleError > 0) {
    //    // 向右转
    //    Motor_SelfRight(turnPID.output);
    //} else if (angleError < 0) {
    //    // 向左转
    //    Motor_SelfLeft(turnPID.output);
    //}
    
    // 判断是否转弯完成
    if (fabs(angleError) < 2.0) {  // 假设2度误差即为完成
        Motor_Stop();  // 停止电机
    }
}

// 启动右转，直到转到90度
void Turn_Right(void) {
    // 通过PID控制器进行右转
    while (fabs(currentAngle) < targetAngle) {  // 目标是90度
        Turn_90_Degrees();  // 持续调用PID控制函数
        Delay_ms(10);  // 每10ms更新一次
    }
    Motor_Stop();  // 当到达90度时，停止电机
}

// 向前直行
void Move_Forward(void) {
    Motor_GoStraight(30);  // 直行，速度设置为30
}

int main(void) {
    // 初始化PID
    PID_Init_Car();
    
    // 初始化MPU6050
    MPU_Init();

    // 初始化电机
    Motor_Init();

    // 初始化OLED
    OLED_Init();

    // 启动时右转90度
    Turn_Right();
    
    // 右转90度后开始直行
    while(1) {
        // 获取当前角度
        Get_Current_Angle();

        // 更新OLED显示
        OLED_ShowString(1, 1, "Target Angle: ");
        OLED_ShowNum(2, 1, (uint32_t)targetAngle, 3);  // 显示目标角度
        OLED_ShowString(3, 1, "Current Angle: ");
        OLED_ShowNum(4, 16, (uint32_t)currentAngle, 3);  // 显示当前角度
        
        // 向前直行
        //Move_Forward();  // 向前直行
        Delay_ms(500);    // 每10ms执行一次
    }
}


