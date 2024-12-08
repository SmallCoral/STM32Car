#include "stm32f10x.h"                  // Device header
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

#define BLACK_LINE_DETECTED 0    	// 红外传感器低电平表示检测到黑线
#define WHITE_SURFACE_DETECTED 1 	// 红外传感器高电平表示检测到白色
 
uint8_t tracking = 0;    					// 巡线模式标志
uint8_t speed = 30;       				// 小车运行速度
uint8_t one = 0;          				// 黑线检测标志，用于确保LED_FMQ()只执行一次

float Pitch,Roll,Yaw;

void OLED_NAME(void)
{
		OLED_ShowChinese(1,1,0);
		OLED_ShowChinese(1,2,1);
		OLED_ShowChinese(1,3,2);
		OLED_ShowChinese(1,4,3);

		OLED_ShowChinese(2,1,5);
		OLED_ShowChinese(2,2,6);
		OLED_ShowChinese(2,3,7);
	
		OLED_ShowChinese(3,1,9);	
		OLED_ShowChinese(3,2,10);
	
		OLED_ShowChinese(4,1,12);
		OLED_ShowChinese(4,2,13);
}

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
            OLED_ShowString(2, 1, "Stopped            ");
						OLED_ShowString(3, 1, "                   ");
            one = 0; // 重置黑线检测标志
        }
    }

    if (tracking && !one) // 小车巡线模式且未检测到黑线
    {
        if (i1 == BLACK_LINE_DETECTED && i2 == BLACK_LINE_DETECTED && 
            i3 == BLACK_LINE_DETECTED && i4 == BLACK_LINE_DETECTED)
        {
            Motor_GoStraight(speed); // 前进
            OLED_ShowString(2, 1, "Moving             ");
						OLED_ShowString(3, 1, "                   ");
        }
        else // 检测到黑线
        {
            Motor_Stop();
            OLED_ShowString(2, 1, "Black Line Detected");
						OLED_ShowString(3, 1, "                   ");
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
            OLED_ShowString(2, 1, "Moving             ");
						OLED_ShowString(3, 1, "                   ");
            Delay_s(3);
            Motor_Stop();
            OLED_ShowString(2, 1, "Stopped            ");
						OLED_ShowString(3, 1, "                   ");
            LED_FMQ();
        }
        else
        {
            Motor_Stop();
            OLED_ShowString(2, 1, "Stopped            ");
						OLED_ShowString(3, 1, "                   ");
        }
    }
}

void made3(void)
{
    static uint8_t tracking = 0; // 追踪状态
    static uint8_t stage = 0;    // 当前执行阶段

    if (GetKeyState(GPIO_Pin_13, GPIOC)) // 检测KEY0
    {
        tracking = !tracking; // 切换追踪状态
        if (!tracking)
        {
            Motor_Stop();
            OLED_ShowString(2, 1, "Stopped            ");
            stage = 0; // 重置阶段
        }
    }

    if (tracking)
    {
        uint8_t i1 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10);
        uint8_t i2 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
        uint8_t i3 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14);
        uint8_t i4 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15);

        switch (stage)
        {
        case 0: // 阶段0：前进直到检测到黑线
            Motor_GoStraight(speed);
            OLED_ShowString(2, 1, "Moving Forward   ");
            if (i1 == BLACK_LINE_DETECTED || i2 == BLACK_LINE_DETECTED ||
                i3 == BLACK_LINE_DETECTED || i4 == BLACK_LINE_DETECTED)
            {
                Motor_Stop();
                OLED_ShowString(2, 1, "Black Line Detected");
                LED_FMQ();
                stage = 1; // 进入下一阶段
            }
            break;

        case 1: // 阶段1：右转1.5秒
            Motor_TurnRight(40);
            OLED_ShowString(2, 1, "Turning Right    ");
            Delay_ms(1500);
            Motor_Stop();
            OLED_ShowString(2, 1, "Turn Complete    ");
            stage = 2; // 进入下一阶段
            break;

        case 2: // 阶段2：前进2.5秒
            Motor_GoStraight(speed);
            OLED_ShowString(2, 1, "Moving Forward   ");
            Delay_ms(2500);
            Motor_Stop();
            OLED_ShowString(2, 1, "Task Complete    ");
            LED_FMQ();
            tracking = 0; // 任务完成，重置追踪状态
            stage = 0; // 重置阶段
            break;

        default:
            Motor_Stop();
            OLED_ShowString(2, 1, "Stopped           ");
            break;
        }
    }
}

void made4(void)
{
    static uint8_t tracking = 0; // 追踪状态
    static uint8_t stage = 0;    // 当前执行阶段

    if (GetKeyState(GPIO_Pin_13, GPIOC)) // 检测KEY0
    {
        tracking = !tracking; // 切换追踪状态
        if (!tracking)
        {
            Motor_Stop();
            OLED_ShowString(2, 1, "Stopped            ");
            stage = 0; // 重置阶段
        }
    }

    if (tracking)
    {
        uint8_t i1 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10);
        uint8_t i2 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
        uint8_t i3 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14);
        uint8_t i4 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15);

        switch (stage)
        {
        case 0: // 阶段0：前进直到检测到黑线
            Motor_GoStraight(speed);
            OLED_ShowString(2, 1, "Moving Forward   ");
            if (i1 == BLACK_LINE_DETECTED && i2 == BLACK_LINE_DETECTED &&
                i3 == BLACK_LINE_DETECTED && i4 == BLACK_LINE_DETECTED)
            {
                Motor_Stop();
                OLED_ShowString(2, 1, "Black Line Detected");
                LED_FMQ();
                stage = 1; // 进入下一阶段
            }
            break;

        case 1: // 阶段1：右转1600ms
            Motor_TurnRight(40);
            OLED_ShowString(2, 1, "Turning Right    ");
            Delay_ms(1600);
            Motor_Stop();
            stage = 2; // 进入下一阶段
            break;

        case 2: // 阶段2：前进3秒
            Motor_GoStraight(speed);
            OLED_ShowString(2, 1, "Moving Forward   ");
            Delay_s(3);
            Motor_Stop();
            OLED_ShowString(2, 1, "Stopped           ");
            LED_FMQ();
            stage = 3; // 进入下一阶段
            break;

        case 3: // 阶段3：左转1600ms
            Motor_TurnLeft(40);
            OLED_ShowString(2, 1, "Turning Left     ");
            Delay_ms(1600);
            Motor_Stop();
            stage = 4; // 进入下一阶段
            break;

        case 4: // 阶段4：前进直到检测到黑线
            Motor_GoStraight(speed);
            OLED_ShowString(2, 1, "Searching Black  ");
            if (i1 == BLACK_LINE_DETECTED && i2 == BLACK_LINE_DETECTED &&
                i3 == BLACK_LINE_DETECTED && i4 == BLACK_LINE_DETECTED)
            {
                Motor_Stop();
                OLED_ShowString(2, 1, "Black Line Detected");
                LED_FMQ();
                stage = 5; // 进入下一阶段
            }
            break;

        case 5: // 阶段5：左转1600ms
            Motor_TurnLeft(40);
            OLED_ShowString(2, 1, "Turning Left     ");
            Delay_ms(1600);
            Motor_Stop();
            stage = 6; // 进入下一阶段
            break;

        case 6: // 阶段6：前进3秒
            Motor_GoStraight(speed);
            OLED_ShowString(2, 1, "Moving Forward   ");
            Delay_s(3);
            Motor_Stop();
            OLED_ShowString(2, 1, "Stopped           ");
            LED_FMQ();
            tracking = 0; // 任务完成，重置追踪状态
            break;

        default:
            Motor_Stop();
            OLED_ShowString(2, 1, "Stopped           ");
            break;
        }
    }
}


