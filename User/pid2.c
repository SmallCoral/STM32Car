#include "stm32f10x.h"                  // �豸ͷ�ļ�
#include "Delay.h"
#include "OLED.h"
#include "Motor.h"
#include "Infrared.h"
#include "Key.h"
#include "buzzer.h"
#include "LED.h"
#include <math.h>
#include "mpu6050.h"
#include "mpuiic.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "sys.h"
#include "usart.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"

#include "pid.h"

// ����PID������
PID turnPID;  // ����PID�������ṹ��
float targetAngle = 90.0;  // Ŀ��Ƕ�90��
float currentAngle = 0.0;  // ��ǰ�Ƕ�
float angleError = 0.0;  // ��ǰ�Ƕ���Ŀ��Ƕȵ����

// PID����
float Kp = 5.0, Ki = 0.1, Kd = 2.0;  // ����PID����������Kp��Kd
float maxI = 50.0, maxOutput = 50.0;  // PID�������Ļ����޷�������޷�

// ��ʼ��PID
void PID_Init_Car(void) {
    PID_Init(&turnPID, Kp, Ki, Kd, maxI, maxOutput);
}

// ��ȡ��ǰ�Ƕȣ�ͨ��MPU6050��
void Get_Current_Angle(void) {
    short gx, gy, gz;
    // ��ȡ�����ǵĽǶ�
    MPU_Get_Gyroscope(&gx, &gy, &gz);
    // ����gz��ʾ��ǰZ��ĽǶȱ仯���ʣ����Ը���ʵ�����������
    currentAngle += gz * 0.01;  // �����������Ϊ10ms����λΪ��
}

// ����С��ת��90��
void Turn_90_Degrees(void) {
    // ��ȡ��ǰ�Ƕ�
    Get_Current_Angle();
    
    // ����Ƕ����
    angleError = targetAngle - currentAngle;

    // ����PID����
    PID_Calc(&turnPID, targetAngle, currentAngle);
    
    //// ����PID������Ƶ��
    //if (angleError > 0) {
    //    // ����ת
    //    Motor_SelfRight(turnPID.output);
    //} else if (angleError < 0) {
    //    // ����ת
    //    Motor_SelfLeft(turnPID.output);
    //}
    
    // �ж��Ƿ�ת�����
    if (fabs(angleError) < 2.0) {  // ����2����Ϊ���
        Motor_Stop();  // ֹͣ���
    }
}

// ������ת��ֱ��ת��90��
void Turn_Right(void) {
    // ͨ��PID������������ת
    while (fabs(currentAngle) < targetAngle) {  // Ŀ����90��
        Turn_90_Degrees();  // ��������PID���ƺ���
        Delay_ms(10);  // ÿ10ms����һ��
    }
    Motor_Stop();  // ������90��ʱ��ֹͣ���
}

// ��ǰֱ��
void Move_Forward(void) {
    Motor_GoStraight(30);  // ֱ�У��ٶ�����Ϊ30
}

int main(void) {
    // ��ʼ��PID
    PID_Init_Car();
    
    // ��ʼ��MPU6050
    MPU_Init();

    // ��ʼ�����
    Motor_Init();

    // ��ʼ��OLED
    OLED_Init();

    // ����ʱ��ת90��
    Turn_Right();
    
    // ��ת90�Ⱥ�ʼֱ��
    while(1) {
        // ��ȡ��ǰ�Ƕ�
        Get_Current_Angle();

        // ����OLED��ʾ
        OLED_ShowString(1, 1, "Target Angle: ");
        OLED_ShowNum(2, 1, (uint32_t)targetAngle, 3);  // ��ʾĿ��Ƕ�
        OLED_ShowString(3, 1, "Current Angle: ");
        OLED_ShowNum(4, 16, (uint32_t)currentAngle, 3);  // ��ʾ��ǰ�Ƕ�
        
        // ��ǰֱ��
        //Move_Forward();  // ��ǰֱ��
        Delay_ms(500);    // ÿ10msִ��һ��
    }
}

