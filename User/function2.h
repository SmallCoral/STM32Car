#include "function.h"
#include "mpu6050.h"
#include "pid.h"
#include "motor.h"      // ������motor���ƺ���
#include "oled.h"       // ������OLED��ʾ����

PID mypid;  // PID�������ṹ��

// made3������ִ��Ԥ������
void made3(void)
{
    short ax, ay, az;
    short gx, gy, gz;
    float roll, pitch, yaw;
    u8 res;

    // ��ʼ��PID�������������Ѿ�������PID����ز�����
    PID_Init(&mypid, 1.0, 0.1, 0.5, 100, 255);

    // 1. ��ǰ��ʻ3��
    Motor_GoStraight(30);  // �����ٶ�50��ִ��ǰ��
    Delay_ms(3000);        // ����ǰ��3��
    Motor_GoStraight(0);   // ֹͣǰ��

    // 2. ��ȡ��ǰ��̬�����ٶȺ����������ݣ�
    res = MPU_Get_Accelerometer(&ax, &ay, &az);  // ��ȡ���ٶ�����
    if (res == 0)
    {
        // ������̬�Ƕ� (�������Ǹ��ݼ��ٶ�ֵ������pitch��roll�Ƕ�)
        roll = atan2(ay, az) * 180 / 3.14159;    // ʹ�ü��ٶ�ֵ����roll�Ƕ�
        pitch = atan2(-ax, sqrt(ay * ay + az * az)) * 180 / 3.14159;  // ����pitch�Ƕ�

        // ��ʾ��ǰ�Ƕȣ�����OLED_ShowString������������ʾ�ַ����ģ�
        OLED_ShowString(2, 1, "Roll: ");  // ��ʾ��Roll: ��
        OLED_ShowNum(2, 7, (int)roll, 3);  // ��ʾroll�Ƕȣ������λ��
        OLED_ShowString(3, 1, "Pitch: "); // ��ʾ��Pitch: ��
        OLED_ShowNum(3, 8, (int)pitch, 3); // ��ʾpitch�Ƕȣ������λ��
    }

    // 3. ʹ��PID������ת90��
    // Ŀ��Ƕ�Ϊ90�ȣ���ת90�ȣ�
    while (1)
    {
        // ��ȡ��ǰ�Ƕ�
        res = MPU_Get_Gyroscope(&gx, &gy, &gz);  // ��ȡ����������
        if (res == 0)
        {
            // ����yaw�Ƕȣ�����ͨ�����������ݹ���ת�ǣ�
            yaw += gx * 0.0000611;  // ���� gx �ǵ�λΪdps�����ݣ�0.0000611Ϊת��ϵ��

            // ʹ��PID����������ת��
            PID_Calc(&mypid, 90.0, yaw);  // Ŀ��90�ȣ���ǰyawֵ
            int pwm_output = mypid.output;

            // ����PID�������ת���������ͨ��PWM�źſ�����ת��
            if (yaw < 90.0)  // ��yawС��Ŀ��ֵ90�ȣ�������ת
            {
                Motor_SelfRight(pwm_output);
            }
            else  // �ﵽ90�Ⱥ�ֹͣת��
            {
                Motor_SelfRight(0);  // ֹͣ��ת
                break;  // �˳�ѭ��
            }
        }

        Delay_ms(50);  // �ӳ�50ms���������ļ���͹����ת��
    }

    // 4. ��ת��Ϻ��ٴ���ǰ��ʻ
    Motor_GoStraight(30);  // ��ǰ��ʻ
    Delay_ms(3000);        // ����ǰ��3��
    Motor_GoStraight(0);   // ֹͣǰ��
}
