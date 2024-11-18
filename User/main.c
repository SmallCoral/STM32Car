#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Motor.h"
#include "Infrared.h"
#include "Key.h"
#include "buzzer.h" 
#include "LED.h" 

// 红外传感器巡线状态定义
#define BLACK_LINE_DETECTED 0    // 红外传感器低电平表示检测到黑线
#define WHITE_SURFACE_DETECTED 1 // 红外传感器高电平表示检测到白色

uint8_t speed = 40; // 小车运行速度

void LED_FMQ(void)
{
	LED_ON();
	Buzzer_ON();     
  Delay_ms(500);
	LED_OFF();
  Buzzer_OFF();    
  Delay_ms(500); 
	
}

void LineTracking(void)
{
    // 读取红外传感器状态
    uint8_t leftFront = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10); // 左前传感器
    uint8_t rightFront = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14); // 右前传感器

    // 判断传感器状态并控制小车
    if (leftFront == BLACK_LINE_DETECTED && rightFront == BLACK_LINE_DETECTED)
    {
        Motor_GoStraight(speed); // 前进
    }
    else
    {
        Motor_Stop(); // 停止
			  LED_FMQ();
    }
}

int main(void)
{
    uint8_t mode = 0;      // 当前模式 (0: 未选择, 1: 第一题, 2: 第二题)
    uint8_t tracking = 0;  // 巡线模式标志

    // 各个模块初始化
		LED_Init();
    OLED_Init();
    Infrared_Init();
    Motor_Init();
    Key_Init();
		Buzzer_Init();

    OLED_ShowString(1, 1, "Hello");

    while (1)
    {
        // 按下KEY_WK (模式选择)
        if (GetKeyState(GPIO_Pin_0, GPIOA))
        {
            mode++;
            if (mode > 2) mode = 1; // 循环切换模式
            OLED_ShowString(2, 1, mode == 1 ? "Mode 1" : "Mode 2");
        }

        if (mode == 1) // 第一题
        {
            if (GetKeyState(GPIO_Pin_13, GPIOC)) // 检测KEY0
            {
                tracking = !tracking; // 切换巡线模式状态
                if (tracking)
                {
                    OLED_ShowString(3, 1, "Tracking On ");
                }
                else
                {
                    OLED_ShowString(3, 1, "Tracking Off");
                    Motor_Stop(); // 停止小车
                }
            }

            if (tracking)
            {
                LineTracking(); // 执行巡线
            }
        }
        else if (mode == 2) // 第二题
        {
            if (GetKeyState(GPIO_Pin_13, GPIOC)) // 检测KEY0
            {
                static uint8_t moving = 0;
                moving = !moving; // 切换移动状态
                if (moving)
                {
                    Motor_GoStraight(speed);
                    OLED_ShowString(3, 1, "Moving        ");
                }
                else
                {
                    Motor_Stop();
                    OLED_ShowString(3, 1, "Stopped       ");
										LED_FMQ();
                }
            }
        }
    }
}
