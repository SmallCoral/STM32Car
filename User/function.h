#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Motor.h"
#include "Infrared.h"
#include "Key.h"
#include "buzzer.h" 
#include "LED.h" 

#define BLACK_LINE_DETECTED 0    // 红外传感器低电平表示检测到黑线
#define WHITE_SURFACE_DETECTED 1 // 红外传感器高电平表示检测到白色
 
uint8_t tracking = 0;    // 巡线模式标志
uint8_t speed = 30;       // 小车运行速度
uint8_t one = 0;          // 黑线检测标志，用于确保LED_FMQ()只执行一次
uint8_t made3_executed = 0; // 标志变量，用于确保made3只执行一次

void LED_FMQ(void)
{
    LED_ON();
    Buzzer_ON();     
    Delay_ms(500);
    LED_OFF();
    Buzzer_OFF();
    Delay_ms(500); 
}

void made1(void)		//第一题
{
    // 读取红外传感器状态
    uint8_t i1 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10);
    uint8_t i2 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
    uint8_t i3 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14);
    uint8_t i4 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15);

    if (GetKeyState(GPIO_Pin_13, GPIOC)) // 检测KEY0
    {
        tracking = !tracking; // 切换巡线模式状态
        if (!tracking)
        {
            Motor_Stop();
            OLED_ShowString(3, 1, "Stopped         ");
            one = 0; // 重置黑线检测标志
        }
    }

    if (tracking && !one) // 小车巡线模式且未检测到黑线
    {
        if (i1 == BLACK_LINE_DETECTED && i2 == BLACK_LINE_DETECTED && 
            i3 == BLACK_LINE_DETECTED && i4 == BLACK_LINE_DETECTED)
        {
            Motor_GoStraight(speed); // 前进
            OLED_ShowString(3, 1, "Moving          ");
        }
        else // 检测到黑线
        {
            Motor_Stop();
            OLED_ShowString(3, 1, "Black Line Detected");
            LED_FMQ(); // 执行一次LED_FMQ
            one = 1; // 标志设置，确保不再重复执行
        }
    }
}

void made2(void)
{
    if (GetKeyState(GPIO_Pin_13, GPIOC)) // 检测KEY0
    {
        static uint8_t moving = 0;
        moving = !moving; // 切换移动状态
        if (moving)
        {
            Motor_GoStraight(speed);
            OLED_ShowString(3, 1, "Moving          ");
            Delay_s(3);
            Motor_Stop();
            OLED_ShowString(3, 1, "Stopped         ");
            LED_FMQ();
        }
        else
        {
            Motor_Stop();
            OLED_ShowString(3, 1, "Stopped         ");
        }
    }
}

void made3(void)
{
    if (!made3_executed) // 确保 made3 只执行一次
    {
        // 第三题任务要求
        OLED_ShowString(3, 1, "Executing Mode 3");

        // 右转并等待 3 秒
        Motor_SelfRight(20); 
        Delay_s(3); // 等待 3 秒

        // 前进并等待 3 秒
        Motor_GoStraight(20); // 使用较低的速度前进
        Delay_s(3); // 等待 3 秒

        // 再次右转并等待 3 秒
        Motor_SelfRight(20); 
        Delay_s(3); // 等待 3 秒

        // 停止小车
        Motor_Stop(); 

        // 设置标志变量，确保只执行一次
        made3_executed = 1; 
        OLED_ShowString(3, 1, "Made3 Executed");
    }
}
