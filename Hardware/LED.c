#include "stm32f10x.h"
#include "PWM.h"

#define LOWEST_BRI 100  // 最低亮度对应CCR
#define HIGHEST_BRI 200 // 最高亮度对应CCR

LED_Init(void)
{
    PWM_Init();
}

/*取值从1到100, 为亮度, 若输入非正数则关闭灯光*/
void LED_SetBrightness(uint16_t Brightness)
{
    if (Brightness > 100)
        Brightness = 100;
    else if (Brightness <= 0)
    {
        TIM_SetCompare1(TIM2, 0);
        return;
    }
    TIM_SetCompare1(TIM2, LOWEST_BRI + (HIGHEST_BRI - LOWEST_BRI) * Brightness / 100);
}

void LED_TurnOff(void)
{
    TIM_SetCompare1(TIM2, 0);
}
