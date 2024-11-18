#include "stm32f10x.h"                  // Device header
#include "DELAY.h"

void Key_Init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;

    // 配置PA0为下拉输入 (KEY_WK)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 配置PC13为下拉输入 (KEY0)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

uint8_t GetKeyState(uint16_t GPIO_Pin, GPIO_TypeDef* GPIOx)
{
    if (GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == 1) // 下拉输入时检测高电平
    {
        Delay_ms(20); // 消抖
        if (GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == 1)
        {
            while (GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == 1); // 等待松开
            Delay_ms(20);
            return 1; // 按键有效
        }
    }
    return 0; // 按键无效
}
