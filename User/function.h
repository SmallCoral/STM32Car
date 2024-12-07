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

#define BLACK_LINE_DETECTED 0    	// ���⴫�����͵�ƽ��ʾ��⵽����
#define WHITE_SURFACE_DETECTED 1 	// ���⴫�����ߵ�ƽ��ʾ��⵽��ɫ
 
uint8_t tracking = 0;    					// Ѳ��ģʽ��־
uint8_t speed = 30;       				// С�������ٶ�
uint8_t one = 0;          				// ���߼���־������ȷ��LED_FMQ()ִֻ��һ��

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

void made1(void)		//��һ��
{
    // ��ȡ���⴫����״̬
    uint8_t i1 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10);
    uint8_t i2 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
    uint8_t i3 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14);
    uint8_t i4 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15);

    if (GetKeyState(GPIO_Pin_13, GPIOC)) // ���KEY0
    {
        tracking = !tracking; // �л�Ѳ��ģʽ״̬
        if (!tracking)
        {
            Motor_Stop();
            OLED_ShowString(2, 1, "Stopped            ");
						OLED_ShowString(3, 1, "                   ");
            one = 0; // ���ú��߼���־
        }
    }

    if (tracking && !one) // С��Ѳ��ģʽ��δ��⵽����
    {
        if (i1 == BLACK_LINE_DETECTED && i2 == BLACK_LINE_DETECTED && 
            i3 == BLACK_LINE_DETECTED && i4 == BLACK_LINE_DETECTED)
        {
            Motor_GoStraight(speed); // ǰ��
            OLED_ShowString(2, 1, "Moving             ");
						OLED_ShowString(3, 1, "                   ");
        }
        else // ��⵽����
        {
            Motor_Stop();
            OLED_ShowString(2, 1, "Black Line Detected");
						OLED_ShowString(3, 1, "                   ");
            LED_FMQ(); // ִ��һ��LED_FMQ
            one = 1; // ��־���ã�ȷ�������ظ�ִ��
        }
    }
}

void made2(void)
{
    if (GetKeyState(GPIO_Pin_13, GPIOC)) // ���KEY0
    {
        static uint8_t moving = 0;
        moving = !moving; // �л��ƶ�״̬
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
    static uint8_t executed = 0;  // ��־����������ȷ��ִֻ��һ��

    if (GetKeyState(GPIO_Pin_13, GPIOC) && !executed)  // ���KEY0����ȷ��δִ�й�
    {
        // 1. С��ǰ��ֱ����⵽����
        Motor_GoStraight(speed);
        while (1) {
            uint8_t i1 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10);
            uint8_t i2 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
            uint8_t i3 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14);
            uint8_t i4 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15);
            if (i1 == WHITE_SURFACE_DETECTED || i2 == WHITE_SURFACE_DETECTED || i3 == WHITE_SURFACE_DETECTED || i4 == WHITE_SURFACE_DETECTED) {
                Motor_Stop();  // ֹͣС��
                OLED_ShowString(2, 1, "Black Line Detected");
                OLED_ShowString(3, 1, "                ");
                LED_FMQ();  // ִ��һ��LED_FMQ
                Delay_ms(500); // ��΢��ʱ����ֹ״̬���ȶ�
                break;  // ����ѭ����������һ������
            }
        }

        // 2. ��ת800ms
        Motor_TurnRight(40);
        Delay_ms(1600);
        
        // 3. ǰ��ֱ����⵽����
        Motor_GoStraight(speed);
        while (1) {
            uint8_t i1 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10);
            uint8_t i2 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
            uint8_t i3 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14);
            uint8_t i4 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15);
            if (i1 == BLACK_LINE_DETECTED && i2 == BLACK_LINE_DETECTED &&
                i3 == BLACK_LINE_DETECTED && i4 == BLACK_LINE_DETECTED) {
                Motor_Stop();  // ֹͣС��
                OLED_ShowString(2, 1, "White Surface Detected");
                OLED_ShowString(3, 1, "                   ");
                LED_FMQ();  // ִ��һ��LED_FMQ
                //Delay_ms(500); // ��΢��ʱ����ֹ״̬���ȶ�
                break;  // ����ѭ����������һ������
            }
        }

        // 4. ��ת800ms
        Motor_TurnRight(40);
        Delay_ms(1600);

        // 5. ǰ��ֱ����⵽����
        Motor_GoStraight(speed);
        while (1) {
            uint8_t i1 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10);
            uint8_t i2 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
            uint8_t i3 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14);
            uint8_t i4 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15);
            if (i1 == WHITE_SURFACE_DETECTED || i2 == WHITE_SURFACE_DETECTED || i3 == WHITE_SURFACE_DETECTED || i4 == WHITE_SURFACE_DETECTED) {
                Motor_Stop();  // ֹͣС��
                OLED_ShowString(2, 1, "Black Line Detected");
                OLED_ShowString(3, 1, "                ");
                LED_FMQ();  // ִ��һ��LED_FMQ
                //Delay_ms(500); // ��΢��ʱ����ֹ״̬���ȶ�
                break;  // ����ѭ����������һ������
            }
        }

        // 6. ��ת800ms
        Motor_TurnRight(40);
        Delay_ms(1600);

        // 7. ǰ��ֱ����⵽���߲�ֹͣ
        Motor_GoStraight(speed);
        while (1) {
            uint8_t i1 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10);
            uint8_t i2 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
            uint8_t i3 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14);
            uint8_t i4 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15);
            if (i1 == BLACK_LINE_DETECTED && i2 == BLACK_LINE_DETECTED &&
                i3 == BLACK_LINE_DETECTED && i4 == BLACK_LINE_DETECTED) {
                Motor_Stop();  // ֹͣС��
                OLED_ShowString(2, 1, "White Surface Detected");
                OLED_ShowString(3, 1, "                   ");
                LED_FMQ();  // ִ��һ��LED_FMQ
                break;  // ֹͣ���в���
            }
        }

        // ���ñ�־��ȷ���Ժ���ִ��
        executed = 1;

        // ֹͣС������ʾ״̬
        Motor_Stop();
        OLED_ShowString(2, 1, "Stopped            ");
        OLED_ShowString(3, 1, "                   ");
        LED_FMQ();
    }
}


	
