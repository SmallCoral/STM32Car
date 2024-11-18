#ifndef __Motor_H__
#define __Motor_H__

void Motor_Init(void);

void Motor_TurnRight(int8_t Speed);
void Motor_TurnLeft(int8_t Speed);
void Motor_GoStraight(int8_t Speed);
void Motor_Stop(void);
void Motor_SelfRight(int8_t Speed);
void Motor_SelfLeft(int8_t Speed);




#endif
