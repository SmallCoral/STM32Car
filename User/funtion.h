#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Motor.h"
#include "Infrared.h"
#include "Key.h"
#include "buzzer.h" 
#include "LED.h" 

#define BLACK_LINE_DETECTED 0    // ���⴫�����͵�ƽ��ʾ��⵽����
#define WHITE_SURFACE_DETECTED 1 // ���⴫�����ߵ�ƽ��ʾ��⵽��ɫ
 
uint8_t tracking = 0;  // Ѳ��ģʽ��־
uint8_t speed = 30;     // С�������ٶ�
uint8_t black_detected = 0; // ���߼���־������ȷ��LED_FMQ()ִֻ��һ��

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
    uint8_t i1 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10); // ��ǰ������
    uint8_t i2 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11); // ��ǰ������
    uint8_t i3 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14);
    uint8_t i4 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15);

    if (GetKeyState(GPIO_Pin_13, GPIOC)) // ���KEY0
    {
        tracking = !tracking; // �л�Ѳ��ģʽ״̬
        if (!tracking)
        {
            Motor_Stop();
            OLED_ShowString(3, 1, "Stopped         ");
            black_detected = 0; // ���ú��߼���־
        }
    }

    if (tracking && !black_detected) // С��Ѳ��ģʽ��δ��⵽����
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
            black_detected = 1; // ��־���ã�ȷ�������ظ�ִ��
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
