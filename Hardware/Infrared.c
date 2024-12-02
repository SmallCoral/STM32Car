#include "stm32f10x.h"                  // Device header
#include "Motor.h"
#include "Delay.h"
//红外循迹 右 PB14 PB15 左 PB10 PB11     接收到信号则为低电平，如果红外下方有黑色胶带，信号就不能返回
//红外安装的高度不要太高，否则红外一直是高电平
/*extern*/ uint8_t speed;      //速度大小可以自己定义
void Infrared_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

void Infrared_Go(void)
{
	Motor_Init();
	if(	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)==0 &&
		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0 &&
		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)==0 &&
		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==0)
	{
		Motor_GoStraight(15);
	}
	if(	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)==1 &&
		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0 &&
		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)==0 &&
		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==0)
	{
		Motor_SelfLeft(speed);
	}
	if(	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)==0 &&
		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==1 &&
		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)==0 &&
		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==0)
	{
		Motor_SelfLeft(speed);
	}
	if(	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)==1 &&
		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==1 &&
		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)==0 &&
		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==0)
	{
		Motor_SelfLeft(speed);
	}
	if(	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)==0 &&
		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0 &&
		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)==1 &&
		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==0)
	{
		Motor_TurnRight(speed);
	}
	if(	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)==0 &&
		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0 &&
		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)==0 &&
		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==1)
	{
		Motor_SelfRight(speed);
	}
	if(	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)==0 &&
		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0 &&
		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)==1 &&
		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==1)
	{
		Motor_SelfRight(speed);
	}
	
	if(	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)==1 &&
		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==1 &&
		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)==1 &&
		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==1)
	{
		Motor_Stop();
	}
}


