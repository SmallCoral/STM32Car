#include "stm32f10x.h"                  // Device header
#include "pid.h"

 
//���ڳ�ʼ��pid�����ĺ���
void PID_Init(PID *pid,float p,float i,float d,float maxI,float maxOut)
{
    pid->kp=p;
    pid->ki=i;
    pid->kd=d;
    pid->maxIntegral=maxI;//�����޷�
    pid->maxOutput=maxOut;//����޷�
}
 
//����һ��pid����
//����Ϊ(pid�ṹ��,Ŀ��ֵ,����ֵ)������������pid�ṹ���output��Ա��
void PID_Calc(PID *pid,float reference,float feedback)
{
	float dout,pout;
 	//��������
    pid->lastError=pid->error;//����error������
    pid->error=reference-feedback;//������error
	pid->error=pid->error;
    //����΢��    //Kd*(���֮��)
    dout=(pid->error-pid->lastError)*pid->kd;
    //�������    //Kp*���
    pout=pid->error*pid->kp;
    //�������    //Ki*�����ۼ�
    pid->integral+=pid->error*pid->ki;
    //�����޷�
    if(pid->integral > pid->maxIntegral) pid->integral=pid->maxIntegral;
    else if(pid->integral < -pid->maxIntegral) pid->integral=-pid->maxIntegral;
    //�������
    pid->output=pout+dout+pid->integral;
    //����޷�
    if(pid->output > pid->maxOutput) pid->output=pid->maxOutput;
    else if(pid->output < -pid->maxOutput) pid->output=-pid->maxOutput;
}
 



void PID_filter(PID *pid,float reference,float feedback)
{
	float dout,pout;
 	//��������
    pid->lastError=pid->error;//����error������
    pid->error=reference-feedback;//������error
	
	pid->error=pid->error*0.8+0.2*pid->lastError;//�˲�
	
    //����΢��    //Kd*(���֮��)
    dout=(pid->error-pid->lastError)*pid->kd;
    //�������    //Kp*���
    pout=pid->error*pid->kp;
    //�������    //Ki*�����ۼ�
    pid->integral+=pid->error*pid->ki;
    //�����޷�
    if(pid->integral > pid->maxIntegral) pid->integral=pid->maxIntegral;
    else if(pid->integral < -pid->maxIntegral) pid->integral=-pid->maxIntegral;
    //�������
    pid->output=pout+dout+pid->integral;
    //����޷�
    if(pid->output > pid->maxOutput) pid->output=pid->maxOutput;
    else if(pid->output < -pid->maxOutput) pid->output=-pid->maxOutput;
}


//PID mypid;//����һ��PID�ṹ�����
// 
//int main()
//{
//    //...������Щ������ʼ������
//    PID_Init(&mypid,10,1,5,800,1000);//��ʼ��PID����
//    while(1)//����ѭ������
//    {
//        float feedbackValue=...;//�����ȡ�����ض���ķ���ֵ
//        float targetValue=...;//�����ȡ��Ŀ��ֵ
//        PID_Calc(&mypid,targetValue,feedbackValue);//����PID���㣬�����output��Ա������
//        �趨ִ���������С(mypid.output);
//        delay(10);//�ȴ�һ��ʱ���ٿ�ʼ��һ��ѭ��
//    }
//}

