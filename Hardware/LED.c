#include "stm32f10x.h" // Device header

void LED_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // ʹ�� GPIOB ʱ��
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      // �������ģʽ
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;            // ���� PB6
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    // ����ٶ� 50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // ��ʼ������������״̬��ȷ�� LED �ر�
    GPIO_SetBits(GPIOB, GPIO_Pin_6); // ���� PB6 Ϊ�͵�ƽ���ر� LED
}

void LED_Turn(void)
{
    if (GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_6) == 0) // ��鵱ǰ״̬
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_6); // ���øߵ�ƽ������ LED
    }
    else
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_6); // ���õ͵�ƽ���ر� LED
    }
}

void LED_ON(void)
{
    GPIO_ResetBits(GPIOB, GPIO_Pin_6); // ���øߵ�ƽ������ LED
}

void LED_OFF(void)
{
    GPIO_SetBits(GPIOB, GPIO_Pin_6); // ���õ͵�ƽ���ر� LED
}
