#include "stm32f10x.h"                  // Device header
#include "PWM.h"
//电机
void Motor_Init(void)
{
	PWM_TIM2Init();
	//初始化GPIO口                                 PWMA对应通道3  PWMB对应通道2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 |GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}
///////////////////////////////////////////////////////////////////////////////////
//  这里左右两边的轮子分别并联连接
//  PA4、PA5接左轮（AO1黑紫 AO2白灰）    PA6、PA7接右轮（BO1黄蓝 BO2红橙）   
//  AIN1 PA4  AIN2 PA5    BIN1 PA7  BIN2 PA6 PWMA PA2 PWMB PA1
//
//  4高5低正转  6高7低正转
//  速度大小范围是0~100，也可以改变PWM初始化中的PSC的值来改变速度
/////////////////////////////////////////////////////////////////////////////////////

//右转    左轮动右轮不动
void Motor_TurnRight(int8_t Speed)
{
	if(Speed>0)
	{
	GPIO_SetBits(GPIOA,GPIO_Pin_4  );
	GPIO_ResetBits(GPIOA,GPIO_Pin_5 );
	GPIO_SetBits(GPIOA,GPIO_Pin_6  );
	GPIO_SetBits(GPIOA,GPIO_Pin_7  );
	PWM_Compare3(Speed);    //设置速度
	}
	else if(Speed<0)
	{
	GPIO_SetBits(GPIOA,GPIO_Pin_5  );
	GPIO_ResetBits(GPIOA,GPIO_Pin_4 );
	GPIO_SetBits(GPIOA,GPIO_Pin_6  );
	GPIO_SetBits(GPIOA,GPIO_Pin_7  );
	PWM_Compare3(-Speed);     //设置速度
	}
}
//左转     左轮不动右轮动
void Motor_TurnLeft(int8_t Speed)
{
	if(Speed>0)
	{	
	GPIO_SetBits(GPIOA,GPIO_Pin_6  );
	GPIO_ResetBits(GPIOA,GPIO_Pin_7 );
	GPIO_SetBits(GPIOA,GPIO_Pin_4  );
	GPIO_SetBits(GPIOA,GPIO_Pin_5  );
	PWM_Compare2(Speed);
	}
	else
	{
	GPIO_SetBits(GPIOA,GPIO_Pin_6  );
	GPIO_ResetBits(GPIOA,GPIO_Pin_7 );
	GPIO_SetBits(GPIOA,GPIO_Pin_4  );
	GPIO_SetBits(GPIOA,GPIO_Pin_5  );
	PWM_Compare2(-Speed);
	}
}
//前进      左右轮都正转
void Motor_GoStraight(int8_t Speed)
{
	if(Speed>0)
	{
	GPIO_SetBits(GPIOA,GPIO_Pin_4  );
	GPIO_ResetBits(GPIOA,GPIO_Pin_5 );
	GPIO_SetBits(GPIOA,GPIO_Pin_6  );
	GPIO_ResetBits(GPIOA,GPIO_Pin_7 );
	PWM_Compare3(Speed);
	PWM_Compare2(Speed);
	}
	else
	{
	GPIO_SetBits(GPIOA,GPIO_Pin_5  );
	GPIO_ResetBits(GPIOA,GPIO_Pin_4 );
	GPIO_SetBits(GPIOA,GPIO_Pin_7  );
	GPIO_ResetBits(GPIOA,GPIO_Pin_6 );
	PWM_Compare3(-Speed);
	PWM_Compare2(-Speed);
	}
}
//停止 
void Motor_Stop(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_5  );
	GPIO_SetBits(GPIOA,GPIO_Pin_4 );
	GPIO_SetBits(GPIOA,GPIO_Pin_7  );
	GPIO_SetBits(GPIOA,GPIO_Pin_6 );
}
//右旋      左轮正转 右轮反转
void Motor_SelfRight(int8_t Speed)
{
	if(Speed>0)
	{
	GPIO_SetBits(GPIOA,GPIO_Pin_4  );
	GPIO_ResetBits(GPIOA,GPIO_Pin_5 );
	GPIO_SetBits(GPIOA,GPIO_Pin_7  );
	GPIO_ResetBits(GPIOA,GPIO_Pin_6  );
	PWM_Compare3(Speed);
	PWM_Compare2(Speed);	
	}
	else if(Speed<0)
	{
	GPIO_SetBits(GPIOA,GPIO_Pin_4  );
	GPIO_ResetBits(GPIOA,GPIO_Pin_5 );
	GPIO_SetBits(GPIOA,GPIO_Pin_7  );
	GPIO_ResetBits(GPIOA,GPIO_Pin_6  );
	PWM_Compare3(-Speed);
	PWM_Compare2(-Speed);	
	}
}
//左旋         左轮反转 右轮正转
void Motor_SelfLeft(int8_t Speed)
{
	if(Speed>0)
	{	
	GPIO_SetBits(GPIOA,GPIO_Pin_6  );
	GPIO_ResetBits(GPIOA,GPIO_Pin_7 );
	GPIO_ResetBits(GPIOA,GPIO_Pin_4  );
	GPIO_SetBits(GPIOA,GPIO_Pin_5  );
	PWM_Compare2(Speed);
	PWM_Compare3(Speed);
	}
	else
	{
	GPIO_SetBits(GPIOA,GPIO_Pin_6  );
	GPIO_ResetBits(GPIOA,GPIO_Pin_7 );
	GPIO_ResetBits(GPIOA,GPIO_Pin_4  );
	GPIO_SetBits(GPIOA,GPIO_Pin_5  );
	PWM_Compare2(-Speed);
	PWM_Compare3(-Speed);
	}
}

