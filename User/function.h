#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Motor.h"
#include "Infrared.h"
#include "Key.h"
#include "buzzer.h" 
#include "LED.h" 
#include "mpu6050.h"
#include "mpuiic.h"
#include "stdio.h"										
#include "string.h"										
#include "stdlib.h"
#include "sys.h"																				
#include "usart.h"																																							
#include "inv_mpu.h"									
#include "inv_mpu_dmp_motion_driver.h"

#define BLACK_LINE_DETECTED 0    // ���⴫�����͵�ƽ��ʾ��⵽����
#define WHITE_SURFACE_DETECTED 1 // ���⴫�����ߵ�ƽ��ʾ��⵽��ɫ
 
uint8_t tracking = 0;    // Ѳ��ģʽ��־
uint8_t speed = 30;       // С�������ٶ�
uint8_t one = 0;          // ���߼���־������ȷ��LED_FMQ()ִֻ��һ��
uint8_t made3_executed = 0; // ��־����������ȷ��made3ִֻ��һ��

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
            OLED_ShowString(3, 1, "Stopped         ");
            one = 0; // ���ú��߼���־
        }
    }

    if (tracking && !one) // С��Ѳ��ģʽ��δ��⵽����
    {
        if (i1 == BLACK_LINE_DETECTED && i2 == BLACK_LINE_DETECTED && 
            i3 == BLACK_LINE_DETECTED && i4 == BLACK_LINE_DETECTED)
        {
            Motor_GoStraight(speed); // ǰ��
            OLED_ShowString(3, 1, "Moving          ");
        }
        else // ��⵽����
        {
            Motor_Stop();
            OLED_ShowString(3, 1, "Black Line Detected");
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
    if (!made3_executed) // ȷ�� made3 ִֻ��һ��
    {
        char display_buffer[16];   // ���� OLED ��ʾ���ַ���������
        short gx, gy, gz;          // ԭʼ�����ǽ��ٶ�����
        float angle_x = 0, angle_y = 0, angle_z = 0; // �ۻ��Ƕ�
        float delta_time = 0.2f;   // ����ÿ�θ���ʱ��Ϊ 200ms��0.2�룩
        
        OLED_ShowString(1, 1, "Reading Angles");

        while (1)
        {
            // ��ȡ�����ǽ��ٶ�����
            MPU_Get_Gyroscope(&gx, &gy, &gz);

            // �ۼƽǶȼ��㣬���� gx/gy/gz ��λΪ��/��
            angle_x += gx * delta_time;
            angle_y += gy * delta_time;
            angle_z += gz * delta_time;

            // ��ʾ X ��Ƕ�
            snprintf(display_buffer, sizeof(display_buffer), "X: %.2f", angle_x);
            OLED_ShowString(1, 2, display_buffer);

            // ��ʾ Y ��Ƕ�
            snprintf(display_buffer, sizeof(display_buffer), "Y: %.2f", angle_y);
            OLED_ShowString(1, 3, display_buffer);

            // ��ʾ Z ��Ƕ�
            snprintf(display_buffer, sizeof(display_buffer), "Z: %.2f", angle_z);
            OLED_ShowString(1, 4, display_buffer);

            // ��ʱ 200 ���룬ģ��ÿ�θ���ʱ��
            Delay_ms(200);
        }
    }
}



