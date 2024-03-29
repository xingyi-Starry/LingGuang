#include "stm32f10x.h"
#include "Timer.h"
#include "Sensor.h"
#include "LED.h"

uint8_t BodyDetect_Flag = 0;
uint8_t Light_Flag = 0;
uint8_t Brightness;
Time time;
Time timing;

/**
 * @brief 时间结构体初始化函数
 *
 * @param time 需要初始化的时间结构体
 * @param mode 初始化模式，DATE表示初始化为日期，TIMING表示初始化为计时器
 * @note TimestructInit(&time, DATE)
 */
void Timer_TimestructInit(Time *time, TIME_MODE mode)
{
    time->year = 0;
    time->hour = 0;
    time->min = 0;
    time->sec = 0;
    if (mode == DATE)
    {
        time->month = 1;
        time->day = 1;
    }
    else
    {
        time->month = 0;
        time->day = 0;
    }
}

void Timer_SetYear(Time *time, uint16_t year)
{
    time->year = year;
}

void Timer_SetMonth(Time *time, uint8_t month)
{
    time->month = month;
}

void Timer_SetDay(Time *time, uint8_t day)
{
    time->day = day;
}

void Timer_SetHour(Time *time, uint8_t hour)
{
    time->hour = hour;
}

void Timer_SetMin(Time *time, uint8_t min)
{
    time->min = min;
}

void Timer_SetSec(Time *time, uint8_t sec)
{
    time->sec = sec;
}

/**
 * @brief 设定时间函数，从指定字符串src设定时间
 *
 * @param time 待设定的时间结构体
 * @param src 时间源字符串，格式为 "AAAA/BB/CC/DD\:EE\:FF"，其中AAAA表年，BB表月，CC表日，DD表时，EE表分，FF表秒
 * @return uint8_t flag 当字符串格式不正确时返回1，否则返回0
 * @note Timer_SetTime(time, Serial_RxPacket)
 */
uint8_t Timer_SetTime(Time *time, char *src)
{
    if (src[4] == '/' && src[7] == '/' && src[10] == '/' && src[13] == ':' && src[16] == ':')
    {
        time->year = (src[0] - '0') * 1000 + (src[1] - '0') * 100 + (src[2] - '0') * 10 + (src[3] - '0');
        time->month = (src[5] - '0') * 10 + (src[6] - '0');
        time->day = (src[8] - '0') * 10 + (src[9] - '0');
        time->hour = (src[11] - '0') * 10 + (src[12] - '0');
        time->min = (src[14] - '0') * 10 + (src[15] - '0');
        time->sec = (src[17] - '0') * 10 + (src[18] - '0');
    }
    else
        return 1;
    return 0;
}

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

    // 初始化时间
    Timer_TimestructInit(&time, DATE);
    Timer_TimestructInit(&timing, TIMING);
}
