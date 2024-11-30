#include "function.h"
#include "mpu6050.h"
#include "pid.h"
#include "motor.h"      // 假设有motor控制函数
#include "oled.h"       // 假设有OLED显示函数

PID mypid;  // PID控制器结构体

// made3函数：执行预定动作
void made3(void)
{
    short ax, ay, az;
    short gx, gy, gz;
    float roll, pitch, yaw;
    u8 res;

    // 初始化PID参数（假设你已经设置了PID的相关参数）
    PID_Init(&mypid, 1.0, 0.1, 0.5, 100, 255);

    // 1. 向前行驶3秒
    Motor_GoStraight(30);  // 设置速度50，执行前进
    Delay_ms(3000);        // 持续前进3秒
    Motor_GoStraight(0);   // 停止前进

    // 2. 获取当前姿态（加速度和陀螺仪数据）
    res = MPU_Get_Accelerometer(&ax, &ay, &az);  // 获取加速度数据
    if (res == 0)
    {
        // 计算姿态角度 (假设我们根据加速度值来估算pitch和roll角度)
        roll = atan2(ay, az) * 180 / 3.14159;    // 使用加速度值计算roll角度
        pitch = atan2(-ax, sqrt(ay * ay + az * az)) * 180 / 3.14159;  // 计算pitch角度

        // 显示当前角度（假设OLED_ShowString函数是用来显示字符串的）
        OLED_ShowString(2, 1, "Roll: ");  // 显示“Roll: ”
        OLED_ShowNum(2, 7, (int)roll, 3);  // 显示roll角度，最多三位数
        OLED_ShowString(3, 1, "Pitch: "); // 显示“Pitch: ”
        OLED_ShowNum(3, 8, (int)pitch, 3); // 显示pitch角度，最多三位数
    }

    // 3. 使用PID控制右转90度
    // 目标角度为90度（右转90度）
    while (1)
    {
        // 获取当前角度
        res = MPU_Get_Gyroscope(&gx, &gy, &gz);  // 获取陀螺仪数据
        if (res == 0)
        {
            // 计算yaw角度（假设通过陀螺仪数据估算转角）
            yaw += gx * 0.0000611;  // 假设 gx 是单位为dps的数据，0.0000611为转换系数

            // 使用PID控制来调整转角
            PID_Calc(&mypid, 90.0, yaw);  // 目标90度，当前yaw值
            int pwm_output = mypid.output;

            // 根据PID输出调整转向（这里假设通过PWM信号控制右转）
            if (yaw < 90.0)  // 若yaw小于目标值90度，继续右转
            {
                Motor_SelfRight(pwm_output);
            }
            else  // 达到90度后停止转向
            {
                Motor_SelfRight(0);  // 停止右转
                break;  // 退出循环
            }
        }

        Delay_ms(50);  // 延迟50ms，避免过快的计算和过大的转动
    }

    // 4. 右转完毕后再次向前行驶
    Motor_GoStraight(30);  // 向前行驶
    Delay_ms(3000);        // 持续前进3秒
    Motor_GoStraight(0);   // 停止前进
}
