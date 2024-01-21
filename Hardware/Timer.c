#include "stm32f10x.h"
#include "Sensor.h"
#include "LED.h"
#include "OLED.h"

uint8_t BodyDetect_Flag = 0;
LED_MODE Mode = AUTO;
LED_STATE State = OFF;
uint8_t Brightness;

uint16_t year = 0;
uint8_t month = 1;
uint8_t day = 1;
uint8_t hour = 0;
uint8_t min = 0;
uint8_t sec = 0;

// TIM3用于实现计时，TIM4用于定时中断

void Timer_Init(void)
{
    // 开启定时器时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    // 选择定时器
    TIM_InternalClockConfig(TIM3);
    TIM_InternalClockConfig(TIM4);

    // 定时器初始化
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 7199;
    TIM_TimeBaseStructure.TIM_Prescaler = 9999;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 7199;
    TIM_TimeBaseStructure.TIM_Prescaler = 99;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    // 使能中断
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

    TIM_ClearFlag(TIM4, TIM_FLAG_Update);
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

    // NVIC初始化
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_Init(&NVIC_InitStructure);

    // 使能定时器
    TIM_Cmd(TIM3, ENABLE);
    TIM_Cmd(TIM4, ENABLE);
}

// 计时
void TIM3_IRQHandler(void)
{
    sec++;
    if (sec >= 60)
    {
        sec = 0;
        min++;
    }
    if (min >= 60)
    {
        min = 0;
        hour++;
    }
    if (hour >= 24)
    {
        hour = 0;
        day++;
    }
    switch (month)
    {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        if (day > 31)
        {
            day = 1;
            month++;
        }
        break;
    case 4:
    case 6:
    case 9:
    case 11:
        if (day > 30)
        {
            day = 1;
            month++;
        }
    case 2:
        if (year % 4 == 0 && year % 100 != 0 || year % 400 == 0)
        {
            if (day > 29)
            {
                day = 1;
                month++;
            }
        }
        else
        {
            if (day > 28)
            {
                day = 1;
                month++;
            }
            break;
        }
    default:
        month = 1;
        break;
    }
    if (month > 12)
    {
        month = 1;
        year++;
    }

    // 显示时间
    OLED_ShowNum(2, 6, year, 4);
    OLED_ShowNum(2, 11, month, 2);
    OLED_ShowNum(2, 14, day, 2);
    OLED_ShowNum(3, 6, hour, 2);
    OLED_ShowNum(3, 9, min, 2);
    OLED_ShowNum(3, 12, sec, 2);

    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
}

void TIM4_IRQHandler(void)
{
    if (LsValue >= LS_EDGE_VALUE /*&& IsValue <= IS_EDGE_VALUE*/)
        BodyDetect_Flag = 1;
    else
        BodyDetect_Flag = 0;

    switch (Mode)
    {
    case AUTO:
        Brightness = 100 * (LsValue - LS_EDGE_VALUE) / (LS_MAX_VALUE - LS_EDGE_VALUE);
        if (BodyDetect_Flag == 1)
            State = ON;
        else
            State = OFF;
        break;
    case MANUAL:
        Brightness = 100;
        break;
    default:
        break;
    }

    switch (State)
    {
    case ON:
        LED_SetBrightness(Brightness);
        break;
    case OFF:
        LED_TurnOff();
        break;
    default:
        break;
    }

    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
}
