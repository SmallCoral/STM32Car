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
    static uint8_t executed = 0;  // 标志变量，用于确保只执行一次

    if (GetKeyState(GPIO_Pin_13, GPIOC) && !executed)  // 检测KEY0并且确保未执行过
    {
        // 1. 小车前进直到检测到黑线
        Motor_GoStraight(speed);
        while (1) {
            uint8_t i1 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10);
            uint8_t i2 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
            uint8_t i3 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14);
            uint8_t i4 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15);
            if (i1 == WHITE_SURFACE_DETECTED || i2 == WHITE_SURFACE_DETECTED || i3 == WHITE_SURFACE_DETECTED || i4 == WHITE_SURFACE_DETECTED) {
                Motor_Stop();  // 停止小车
                OLED_ShowString(2, 1, "Black Line Detected");
                OLED_ShowString(3, 1, "                ");
                LED_FMQ();  // 执行一次LED_FMQ
                Delay_ms(500); // 稍微延时，防止状态不稳定
                break;  // 跳出循环，进行下一个操作
            }
        }

        // 2. 右转800ms
        Motor_TurnRight(40);
        Delay_ms(1600);
        
        // 3. 前进直到检测到白线
        Motor_GoStraight(speed);
        while (1) {
            uint8_t i1 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10);
            uint8_t i2 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
            uint8_t i3 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14);
            uint8_t i4 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15);
            if (i1 == BLACK_LINE_DETECTED && i2 == BLACK_LINE_DETECTED &&
                i3 == BLACK_LINE_DETECTED && i4 == BLACK_LINE_DETECTED) {
                Motor_Stop();  // 停止小车
                OLED_ShowString(2, 1, "White Surface Detected");
                OLED_ShowString(3, 1, "                   ");
                LED_FMQ();  // 执行一次LED_FMQ
                //Delay_ms(500); // 稍微延时，防止状态不稳定
                break;  // 跳出循环，进行下一个操作
            }
        }

        // 4. 右转800ms
        Motor_TurnRight(40);
        Delay_ms(1600);

        // 5. 前进直到检测到黑线
        Motor_GoStraight(speed);
        while (1) {
            uint8_t i1 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10);
            uint8_t i2 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
            uint8_t i3 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14);
            uint8_t i4 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15);
            if (i1 == WHITE_SURFACE_DETECTED || i2 == WHITE_SURFACE_DETECTED || i3 == WHITE_SURFACE_DETECTED || i4 == WHITE_SURFACE_DETECTED) {
                Motor_Stop();  // 停止小车
                OLED_ShowString(2, 1, "Black Line Detected");
                OLED_ShowString(3, 1, "                ");
                LED_FMQ();  // 执行一次LED_FMQ
                //Delay_ms(500); // 稍微延时，防止状态不稳定
                break;  // 跳出循环，进行下一个操作
            }
        }

        // 6. 右转800ms
        Motor_TurnRight(40);
        Delay_ms(1600);

        // 7. 前进直到检测到白线并停止
        Motor_GoStraight(speed);
        while (1) {
            uint8_t i1 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10);
            uint8_t i2 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
            uint8_t i3 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14);
            uint8_t i4 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15);
            if (i1 == BLACK_LINE_DETECTED && i2 == BLACK_LINE_DETECTED &&
                i3 == BLACK_LINE_DETECTED && i4 == BLACK_LINE_DETECTED) {
                Motor_Stop();  // 停止小车
                OLED_ShowString(2, 1, "White Surface Detected");
                OLED_ShowString(3, 1, "                   ");
                LED_FMQ();  // 执行一次LED_FMQ
                break;  // 停止所有操作
            }
        }

        // 设置标志，确保以后不再执行
        executed = 1;

        // 停止小车并显示状态
        Motor_Stop();
        OLED_ShowString(2, 1, "Stopped            ");
        OLED_ShowString(3, 1, "                   ");
        LED_FMQ();
    }
}


	
