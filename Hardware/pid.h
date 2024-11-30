#ifndef __PID_H__
#define __PID_H__

//���ȶ���PID�ṹ�����ڴ��һ��PID������
typedef struct
{
   	float kp,ki,kd;//����ϵ��
    float error,lastError;//���ϴ����
    float integral,maxIntegral;//���֡������޷�
    float output,maxOutput;//���������޷�
}PID;

          //�ṹ�����    KP     KI       KD     �����޷�    ����޷�
void PID_Init(PID *pid,float p,float i,float d,float maxI,float maxOut);

//��ͨPID
         //�ṹ�����         Ԥ��ֵ           ʵ��ֵ      
void PID_Calc(PID *pid,float reference,float feedback);

//��ͨ�˲�
void PID_filter(PID *pid,float reference,float feedback);

#endif
