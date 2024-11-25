#include "funtion.h"

int main(void)
{
    uint8_t mode = 0; // 当前模式 (0: 未选择, 1: 第一题, 2: 第二题)

    // 各个模块初始化
    LED_Init();
    OLED_Init();
    Infrared_Init();
    Motor_Init();
    Key_Init();
    Buzzer_Init();

    OLED_ShowString(1, 1, "Hello");

    while (1)
    {
        // 按下KEY_WK (模式选择)
        if (GetKeyState(GPIO_Pin_0, GPIOA))
        {
            mode++;
            if (mode > 2) mode = 1; // 循环切换模式
            OLED_ShowString(2, 1, mode == 1 ? "Mode 1" : "Mode 2");
            Motor_Stop(); // 切换模式时停止小车
            black_detected = 0; // 重置黑线检测标志
        }

        if (mode == 1) // 第一题
        {
            made1();
        }
        else if (mode == 2) // 第二题
        {
            made2();
        }
    }
}

