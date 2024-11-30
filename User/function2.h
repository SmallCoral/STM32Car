#include "stm32f10x.h"                  // �豸ͷ�ļ�
#include "PWM.h"
#include "function.h"
#include "mpu6050.h"
#include "pid.h"  // ����PID������

// PID ������ʵ��
PID myPID;  // ����һ��PID�ṹ��

// PID ����������
float target_angle = 90.0;  // Ŀ��Ƕ�
float current_angle = 0.0;  // ��ǰ�Ƕ�
float left_speed = 0.0, right_speed = 0.0;  // �������ٶ�

// ��ʼ��PID������
void PID_Init_Controller(void)
{
    PID_Init(&myPID, 1.0, 0.0, 0.0, 100.0, 255.0); // ���� Kp=1.0, Ki=0.0, Kd=0.0
}

// ����PID�������������ٶ�
void update_pid(void)
{
    // ����PID����
    PID_Calc(&myPID, target_angle, current_angle);  // ʹ�õ�ǰ�ǶȺ�Ŀ��Ƕȼ���PID���

    // ����PID������������ֵ�ת��
    left_speed = 0.5 + myPID.output;
    right_speed = 0.5 - myPID.output;
}

// ������ƺ���
void set_motor_speed(void)
{
    // ����PID����������ҵ��ת��
    GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)(left_speed > 0.5));
    GPIO_WriteBit(GPIOA, GPIO_Pin_5, (BitAction)(left_speed < 0.5));
    GPIO_WriteBit(GPIOA, GPIO_Pin_6, (BitAction)(right_speed > 0.5));
    GPIO_WriteBit(GPIOA, GPIO_Pin_7, (BitAction)(right_speed < 0.5));
}

void made3(void)
{
    // ��ʼ���������
    Motor_Init();
    // ��ʼ��PID������
    PID_Init_Controller();

    // ���ó�ʼ״̬����ǰ��
    GPIO_SetBits(GPIOA, GPIO_Pin_4);
    GPIO_ResetBits(GPIOA, GPIO_Pin_5);
    GPIO_SetBits(GPIOA, GPIO_Pin_6);
    GPIO_ResetBits(GPIOA, GPIO_Pin_7);
    Delay_ms(1000);

    // ��ǰ�˶���Ŀ��Ƕ�Ϊ0
    target_angle = 0.0;
    while (fabs(myPID.error) > 0.1)  // �����С��0.1ʱ��Ϊ�Ѿ�����Ŀ��Ƕ�
    {
        update_pid();  // ����PID����
        set_motor_speed();  // ���µ��ת��
        Delay_ms(10);  // �ӳ�10ms
    }

    // ��ǰһ��ʱ��
    GPIO_SetBits(GPIOA, GPIO_Pin_4);
    GPIO_ResetBits(GPIOA, GPIO_Pin_5);
    GPIO_SetBits(GPIOA, GPIO_Pin_6);
    GPIO_ResetBits(GPIOA, GPIO_Pin_7);
    Delay_ms(1000);

    // ��ת��Ŀ��Ƕ�Ϊ90��
    target_angle = 90.0;
    while (fabs(myPID.error) > 0.1)
    {
        update_pid();  // ����PID����
        set_motor_speed();  // ���µ��ת��
        Delay_ms(10);  // �ӳ�10ms
    }

    // ��ǰһ��ʱ��
    GPIO_SetBits(GPIOA, GPIO_Pin_4);
    GPIO_ResetBits(GPIOA, GPIO_Pin_5);
    GPIO_SetBits(GPIOA, GPIO_Pin_6);
    GPIO_ResetBits(GPIOA, GPIO_Pin_7);
    Delay_ms(1000);

    // ��ת��Ŀ��Ƕ�Ϊ90��
    target_angle = -90.0;  // ��ת
    while (fabs(myPID.error) > 0.1)
    {
        update_pid();  // ����PID����
        set_motor_speed();  // ���µ��ת��
        Delay_ms(10);  // �ӳ�10ms
    }

    // ��ǰһ��ʱ��
    GPIO_SetBits(GPIOA, GPIO_Pin_4);
    GPIO_ResetBits(GPIOA, GPIO_Pin_5);
    GPIO_SetBits(GPIOA, GPIO_Pin_6);
    GPIO_ResetBits(GPIOA, GPIO_Pin_7);
    Delay_ms(1000);

}
