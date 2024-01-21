#include "stm32f10x.h"
#include "PWM.h"
#include "LED.h"

LED_Init(void)
{
    PWM_Init();
}

/*取值从0到100, 为亮度*/
void LED_SetBrightness(uint16_t Brightness)
{
    if (Brightness > 100)
        Brightness = 100;
    else if (Brightness < 0)
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
