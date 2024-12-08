#include "function.h"


int main(void)
{
    uint8_t mode = 0; // 当前模式 (0: 未选择, 1: 第一题, 2: 第二题, 3: 第三题)

    // 各个模块初始化
		OLED_Init();
    LED_Init();
    //OLED_Init();
    Infrared_Init();
    Motor_Init();
    Key_Init();
    Buzzer_Init();
    MPU_Init();
		
    OLED_NAME();

    while (1)
    {
        // 按下KEY_WK (模式选择)
        if (GetKeyState(GPIO_Pin_0, GPIOA))
        {
            mode++;
            if (mode > 4) mode = 1; // 循环切换模式 (1, 2, 3，4)

            // 根据模式显示状态
            if (mode == 1)
            {
                OLED_ShowString(1, 1, "Mode 1          ");
                OLED_ShowString(2, 1, "                ");
                OLED_ShowString(3, 1, "                ");
                OLED_ShowString(4, 1, "                ");
            }
            else if (mode == 2)
            {
                OLED_ShowString(1, 1, "Mode 2          ");
                OLED_ShowString(2, 1, "                ");
                OLED_ShowString(3, 1, "                ");
                OLED_ShowString(4, 1, "                ");
            }
            else if (mode == 3)
            {
                OLED_ShowString(1, 1, "Mode 3          ");
                OLED_ShowString(2, 1, "                ");
                OLED_ShowString(3, 1, "                ");
                OLED_ShowString(4, 1, "                ");
            }
						else if (mode == 4)
            {
                OLED_ShowString(1, 1, "Mode 4          ");
                OLED_ShowString(2, 1, "                ");
                OLED_ShowString(3, 1, "                ");
                OLED_ShowString(4, 1, "                ");
            }

            Motor_Stop(); // 切换模式时停止小车
            one = 0; // 重置黑线检测标志
        }

        if (mode == 1) // 第一题
        {
            made1();
        }
        else if (mode == 2) // 第二题
        {
            made2();
        }
        else if (mode == 3) // 第三题
        {
						made3();
        }
				else if (mode == 4) // 第三题
        {
						made4();
        }
    }
}


