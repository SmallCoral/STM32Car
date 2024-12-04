#include "stm32f10x.h"                  // Device header

// 电机PWM初始化，用TIM2
void PWM_TIM2Init(void)
{
	//初始化TIM
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 36 - 1;		//PSC  可改变电机的速度
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_InternalClockConfig(TIM2);
	
	//PWMA
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1; //输出比较模式
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;  //选择有效电平是高还是低
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;    //输出使能
	TIM_OCInitStructure.TIM_Pulse=50;                      //设置CCR寄存器的值
	TIM_OC3Init(TIM2,&TIM_OCInitStructure);
	//PWMB
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1; //输出比较模式
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;  //选择有效电平是高还是低
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;    //输出使能
	TIM_OCInitStructure.TIM_Pulse=50;    
	TIM_OC2Init(TIM2,&TIM_OCInitStructure);
	//开启GPIO时钟
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
	TIM_Cmd(TIM2,ENABLE);
}
//舵机PWM初始化，用TIM3
void PWM_TIM3Init(void)
{

	//初始化TIM
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	TIM_InternalClockConfig(TIM3); //选择内部中断
	
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1; //输出比较模式
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;  //选择有效电平是高还是低
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;    //输出使能
	TIM_OCInitStructure.TIM_Pulse=0;                      //设置CCR寄存器的值
	TIM_OC3Init(TIM3,&TIM_OCInitStructure);
	
	//开启GPIO时钟
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	
	TIM_Cmd(TIM3,ENABLE);

}
//设置速度
void PWM_Compare3(uint16_t Compare)
{
	TIM_SetCompare3(TIM2,Compare);
}
//设置速度
void PWM_Compare2(uint16_t Compare)
{
	TIM_SetCompare2(TIM2,Compare);
}
//舵机PWM
void PWM_TIM3Compare3(uint16_t Compare)
{
	TIM_SetCompare3(TIM3,Compare);
}

