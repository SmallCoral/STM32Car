#ifndef __Key_H__
#define __Key_H__

#include "stm32f10x.h"

void Key_Init(void);
uint8_t GetKeyState(uint16_t GPIO_Pin, GPIO_TypeDef* GPIOx);

#endif
