#ifndef __USART_H__
#define __USART_H__

void USART1_Init(void);
uint8_t USART_GetData(void);
uint8_t USART_GetFlag(void);
void USART_SendByte(uint8_t Byte);
void USART_SendNumber(uint32_t Number, uint8_t Length);


#endif
