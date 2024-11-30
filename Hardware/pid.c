#include "stm32f10x.h"                  // Device header
#include "pid.h"

 
//用于初始化pid参数的函数
void PID_Init(PID *pid,float p,float i,float d,float maxI,float maxOut)
{
    pid->kp=p;
    pid->ki=i;
    pid->kd=d;
    pid->maxIntegral=maxI;//积分限幅
    pid->maxOutput=maxOut;//输出限幅
}
 
//进行一次pid计算
//参数为(pid结构体,目标值,反馈值)，计算结果放在pid结构体的output成员中
void PID_Calc(PID *pid,float reference,float feedback)
{
	float dout,pout;
 	//更新数据
    pid->lastError=pid->error;//将旧error存起来
    pid->error=reference-feedback;//计算新error
	pid->error=pid->error;
    //计算微分    //Kd*(误差之差)
    dout=(pid->error-pid->lastError)*pid->kd;
    //计算比例    //Kp*误差
    pout=pid->error*pid->kp;
    //计算积分    //Ki*误差的累加
    pid->integral+=pid->error*pid->ki;
    //积分限幅
    if(pid->integral > pid->maxIntegral) pid->integral=pid->maxIntegral;
    else if(pid->integral < -pid->maxIntegral) pid->integral=-pid->maxIntegral;
    //计算输出
    pid->output=pout+dout+pid->integral;
    //输出限幅
    if(pid->output > pid->maxOutput) pid->output=pid->maxOutput;
    else if(pid->output < -pid->maxOutput) pid->output=-pid->maxOutput;
}
 



void PID_filter(PID *pid,float reference,float feedback)
{
	float dout,pout;
 	//更新数据
    pid->lastError=pid->error;//将旧error存起来
    pid->error=reference-feedback;//计算新error
	
	pid->error=pid->error*0.8+0.2*pid->lastError;//滤波
	
    //计算微分    //Kd*(误差之差)
    dout=(pid->error-pid->lastError)*pid->kd;
    //计算比例    //Kp*误差
    pout=pid->error*pid->kp;
    //计算积分    //Ki*误差的累加
    pid->integral+=pid->error*pid->ki;
    //积分限幅
    if(pid->integral > pid->maxIntegral) pid->integral=pid->maxIntegral;
    else if(pid->integral < -pid->maxIntegral) pid->integral=-pid->maxIntegral;
    //计算输出
    pid->output=pout+dout+pid->integral;
    //输出限幅
    if(pid->output > pid->maxOutput) pid->output=pid->maxOutput;
    else if(pid->output < -pid->maxOutput) pid->output=-pid->maxOutput;
}


//PID mypid;//创建一个PID结构体变量
// 
//int main()
//{
//    //...这里有些其他初始化代码
//    PID_Init(&mypid,10,1,5,800,1000);//初始化PID参数
//    while(1)//进入循环运行
//    {
//        float feedbackValue=...;//这里获取到被控对象的反馈值
//        float targetValue=...;//这里获取到目标值
//        PID_Calc(&mypid,targetValue,feedbackValue);//进行PID计算，结果在output成员变量中
//        设定执行器输出大小(mypid.output);
//        delay(10);//等待一定时间再开始下一次循环
//    }
//}

