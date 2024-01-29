#include "stm32f10x.h"
#include "PWM.h"
#include "LED.h"

LED_MODE Mode = AUTO;
LED_STATE State = OFF;

void LED_SwitchMode(void)
{
    if (Mode == AUTO)
        Mode = MANUAL;
    else
        Mode = AUTO;
}

void LED_SwitchState(void)
{
    if (State == OFF)
        State = ON;
    else
        State = OFF;
}

void LED_Init(void)
{
    PWM_Init();
}

/**
 * @brief 设置LED亮度
 * 
 * @param Brightness 亮度，取值从0到100
 * @note LED_SetBrightness(50)
 */
void LED_SetBrightness(uint16_t Brightness)
{
    if (Brightness > 100)
        Brightness = 100;
    TIM_SetCompare1(TIM2, LOWEST_BRI + (HIGHEST_BRI - LOWEST_BRI) * Brightness / 100);
}

/**
 * @brief 关闭LED
 * 
 * @note LED_TurnOff()
 */
void LED_TurnOff(void)
{
    TIM_SetCompare1(TIM2, 0);
}
