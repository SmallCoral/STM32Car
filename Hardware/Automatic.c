#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "U1trasound.h"
#include "Engine.h"
#include "Motor.h"

//uint8_t Distance1=0;
//uint8_t Distance2=0;
//uint8_t Distance3=0;

void Automatic_Init(void)
{
	OLED_Init();
	Motor_Init();
	Engine_Init();
	U1trasound_Init();
}

void Automatic_Car(void)
{
	
//	Engine_SetAngle(90);
//	Motor_GoStraight(70);
//	Distance1=U1trasound_GetDistance();
//	OLED_ShowNum(1,10,Distance1,2);
//	Delay_ms(100);
//	if(Distance1<20)
//	{
//		Motor_Stop();
//		Engine_SetAngle(180);
//		Delay_ms(1000);
//		
//		Distance2=U1trasound_GetDistance();
//		OLED_ShowNum(2,6,Distance2,2);
//		
//		if(Distance2>20 )
//		{
//			
//			Motor_SelfLeft(70);
//			Delay_ms(1000);
//			Motor_GoStraight(70);
//		}
//		else
//		{
//			Engine_SetAngle(0);
//			Delay_ms(1000);
//			Distance3=U1trasound_GetDistance();
//			OLED_ShowNum(3,7,Distance3,2);
//			Engine_SetAngle(90);
//			if(Distance3>20 )
//			{
//				Motor_SelfRight(70);
//				Delay_ms(1000);
//				Motor_GoStraight(70);
//			}
//			else
//			{
//				Motor_GoStraight(-70);
//				Delay_ms(1000);
//				Motor_GoStraight(70);
//			}
//		}	
//	}
//	Delay_ms(100);
}


