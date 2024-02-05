#include "stm32f10x.h"
#include "PWM.h"
#include "LED.h"

LED_MODE Mode = AUTO;
LED_STATE State = OFF;
uint8_t Man_Bri = 100;

/**
 * @brief LED初始化
 * 
 */
void LED_Init(void)
{
    PWM_Init();
}

/**
 * @brief 切换至自动模式
 * 
 */
void LED_AutoMode(void)
{
    Mode = AUTO;
}

/**
 * @brief 切换至手动模式
 * 
 */
void LED_ManualMode(void)
{
    Mode = MANUAL;
}

/**
 * @brief LED模式切换函数，可在手动模式和自动模式间切换
 *
 */
void LED_SwitchMode(void)
{
    if (Mode == AUTO)
        Mode = MANUAL;
    else
        Mode = AUTO;
}

/**
 * @brief LED状态切换函数
 *
 * @note 仅在手动模式下有效
 * @return uint8_t flag 当处于手动模式切换成功时返回0，否则返回1
 */
uint8_t LED_SwitchState(void)
{
    if (Mode == MANUAL)
    {
        if (State == OFF)
            State = ON;
        else
        {
            State = OFF;
        }
    }
    else 
        return 1;
    return 0;
}

/**
 * @brief 设置LED亮度
 *
 * @param Brightness 亮度，取值从0到100
 */
void LED_SetBrightness(uint16_t Brightness)
{
    if (Brightness > 100)
        Brightness = 100;
    TIM_SetCompare1(TIM2, LOWEST_BRI + (HIGHEST_BRI - LOWEST_BRI) * Brightness / 100);
}

/**
 * @brief 打开LED
 * 切换至手动模式并打开LED
 * 
 */
void LED_TurnOn(void)
{
    Mode = MANUAL;
    State = ON;
}

/**
 * @brief 关闭LED
 * 切换至手动模式并关闭LED
 *
 */
void LED_TurnOff(void)
{
    Mode = MANUAL;
    State = OFF;
}

/**
 * @brief LED提高亮度
 * 
 * @note 仅在手动模式且开灯状态下有效
 * @return uint8_t flag 成功提高亮度时返回0，处于自动模式时返回1，未开灯时返回2，已达最高亮度返回3
 */
uint8_t LED_BriUp(void)
{
    if (Mode == AUTO)
        return 1;
    if (State == OFF)
        return 2;
    if (Man_Bri < 100)
        Man_Bri += 10;
    else
        return 3;
    return 0;
}

/**
 * @brief LED降低亮度
 * 
 * @note 仅在手动模式且开灯状态下有效
 * @return uint8_t flag 成功降低亮度时返回0，处于自动模式时返回1，已达最低亮度返回2
 */
uint8_t LED_BriDown(void)
{
    if (Mode == AUTO)
        return 1;
    if (State == OFF)
        return 2;
    if (Man_Bri > 10)
        Man_Bri -= 10;
    else
        return 3;
    return 0;
}
