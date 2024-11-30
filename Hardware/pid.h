#ifndef __PID_H__
#define __PID_H__

//首先定义PID结构体用于存放一个PID的数据
typedef struct
{
   	float kp,ki,kd;//三个系数
    float error,lastError;//误差、上次误差
    float integral,maxIntegral;//积分、积分限幅
    float output,maxOutput;//输出、输出限幅
}PID;

          //结构体变量    KP     KI       KD     积分限幅    输出限幅
void PID_Init(PID *pid,float p,float i,float d,float maxI,float maxOut);

//普通PID
         //结构体变量         预期值           实际值      
void PID_Calc(PID *pid,float reference,float feedback);

//低通滤波
void PID_filter(PID *pid,float reference,float feedback);

#endif
