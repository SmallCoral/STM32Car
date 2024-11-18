#include "stm32f10x.h" // Device header

void LED_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 使能 GPIOB 时钟
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      // 推挽输出模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;            // 配置 PB6
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    // 输出速度 50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 初始化后设置引脚状态，确保 LED 关闭
    GPIO_SetBits(GPIOB, GPIO_Pin_6); // 设置 PB6 为低电平，关闭 LED
}

void LED_Turn(void)
{
    if (GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_6) == 0) // 检查当前状态
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_6); // 设置高电平，点亮 LED
    }
    else
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_6); // 设置低电平，关闭 LED
    }
}

void LED_ON(void)
{
    GPIO_ResetBits(GPIOB, GPIO_Pin_6); // 设置高电平，点亮 LED
}

void LED_OFF(void)
{
    GPIO_SetBits(GPIOB, GPIO_Pin_6); // 设置低电平，关闭 LED
}
