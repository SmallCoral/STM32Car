#include "stm32f10x.h"                  // Device header
#include "DELAY.h"

void Key_Init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;

    // ����PA0Ϊ�������� (KEY_WK)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // ����PC13Ϊ�������� (KEY0)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

uint8_t GetKeyState(uint16_t GPIO_Pin, GPIO_TypeDef* GPIOx)
{
    if (GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == 1) // ��������ʱ���ߵ�ƽ
    {
        Delay_ms(20); // ����
        if (GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == 1)
        {
            while (GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == 1); // �ȴ��ɿ�
            Delay_ms(20);
            return 1; // ������Ч
        }
    }
    return 0; // ������Ч
}
