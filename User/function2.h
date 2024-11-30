#include "stm32f10x.h"                  // 设备头文件
#include "PWM.h"
#include "function.h"
#include "mpu6050.h"
#include "pid.h"  // 引入PID控制器

// PID 控制器实例
PID myPID;  // 创建一个PID结构体

// PID 控制器参数
float target_angle = 90.0;  // 目标角度
float current_angle = 0.0;  // 当前角度
float left_speed = 0.0, right_speed = 0.0;  // 左右轮速度

// 初始化PID控制器
void PID_Init_Controller(void)
{
    PID_Init(&myPID, 1.0, 0.0, 0.0, 100.0, 255.0); // 假设 Kp=1.0, Ki=0.0, Kd=0.0
}

// 更新PID并计算左右轮速度
void update_pid(void)
{
    // 进行PID计算
    PID_Calc(&myPID, target_angle, current_angle);  // 使用当前角度和目标角度计算PID输出

    // 根据PID输出调整左右轮的转速
    left_speed = 0.5 + myPID.output;
    right_speed = 0.5 - myPID.output;
}

// 电机控制函数
void set_motor_speed(void)
{
    // 根据PID控制输出左右电机转速
    GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)(left_speed > 0.5));
    GPIO_WriteBit(GPIOA, GPIO_Pin_5, (BitAction)(left_speed < 0.5));
    GPIO_WriteBit(GPIOA, GPIO_Pin_6, (BitAction)(right_speed > 0.5));
    GPIO_WriteBit(GPIOA, GPIO_Pin_7, (BitAction)(right_speed < 0.5));
}

void made3(void)
{
    // 初始化电机控制
    Motor_Init();
    // 初始化PID控制器
    PID_Init_Controller();

    // 设置初始状态（向前）
    GPIO_SetBits(GPIOA, GPIO_Pin_4);
    GPIO_ResetBits(GPIOA, GPIO_Pin_5);
    GPIO_SetBits(GPIOA, GPIO_Pin_6);
    GPIO_ResetBits(GPIOA, GPIO_Pin_7);
    Delay_ms(1000);

    // 向前运动，目标角度为0
    target_angle = 0.0;
    while (fabs(myPID.error) > 0.1)  // 当误差小于0.1时认为已经到达目标角度
    {
        update_pid();  // 更新PID计算
        set_motor_speed();  // 更新电机转速
        Delay_ms(10);  // 延迟10ms
    }

    // 向前一段时间
    GPIO_SetBits(GPIOA, GPIO_Pin_4);
    GPIO_ResetBits(GPIOA, GPIO_Pin_5);
    GPIO_SetBits(GPIOA, GPIO_Pin_6);
    GPIO_ResetBits(GPIOA, GPIO_Pin_7);
    Delay_ms(1000);

    // 左转，目标角度为90度
    target_angle = 90.0;
    while (fabs(myPID.error) > 0.1)
    {
        update_pid();  // 更新PID计算
        set_motor_speed();  // 更新电机转速
        Delay_ms(10);  // 延迟10ms
    }

    // 向前一段时间
    GPIO_SetBits(GPIOA, GPIO_Pin_4);
    GPIO_ResetBits(GPIOA, GPIO_Pin_5);
    GPIO_SetBits(GPIOA, GPIO_Pin_6);
    GPIO_ResetBits(GPIOA, GPIO_Pin_7);
    Delay_ms(1000);

    // 右转，目标角度为90度
    target_angle = -90.0;  // 右转
    while (fabs(myPID.error) > 0.1)
    {
        update_pid();  // 更新PID计算
        set_motor_speed();  // 更新电机转速
        Delay_ms(10);  // 延迟10ms
    }

    // 向前一段时间
    GPIO_SetBits(GPIOA, GPIO_Pin_4);
    GPIO_ResetBits(GPIOA, GPIO_Pin_5);
    GPIO_SetBits(GPIOA, GPIO_Pin_6);
    GPIO_ResetBits(GPIOA, GPIO_Pin_7);
    Delay_ms(1000);

}
