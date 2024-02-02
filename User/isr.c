#include <string.h>
#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "LED.h"
#include "OLED.h"
#include "Key.h"
#include "Timer.h"
#include "Sensor.h"
#include "Serial.h"

/**
 * @brief 命令处理函数
 * 虽然不是中断，但是不知道应该放哪了，就先放这吧
 * 
 * @param RxPacket 命令源
 */
void Serial_CmdHandler(char *RxPacket)
{
    // 输入时间
    if (RxPacket[4] == '/' && RxPacket[7] == '/' && RxPacket[10] == '/' && RxPacket[13] == ':' && RxPacket[16] == ':')
    {
        if (Timer_SetTime(&time, RxPacket) == 1)
            Serial_SendString("Wrong time\r\n", SERIAL_BT);
        else
            Serial_SendString("Time set success\r\n", SERIAL_BT);
    }
    // 切换模式
    else if (strcmp(RxPacket, "SwitchMode") == 0)
    {
        LED_SwitchMode();
        Serial_SendString("Mode switch successful\r\n", SERIAL_BT);
    }
    // 切换状态
    else if (strcmp(RxPacket, "SwitchState") == 0)
    {
        if (LED_SwitchState() == 1)
            Serial_SendString("Error: please switch to mannual mode first\r\n", SERIAL_BT);
        else
            Serial_SendString("State switch successful\r\n", SERIAL_BT);
    }
    // 获取学习时间
    else if (strcmp(RxPacket, "GetLearningTime") == 0)
    {
        Serial_SendNumber(timing.hour, SERIAL_BT);
        Serial_SendString(":", SERIAL_BT);
        Serial_SendNumber(timing.min, SERIAL_BT);
        Serial_SendString(":", SERIAL_BT);
        Serial_SendNumber(timing.sec, SERIAL_BT);
        Serial_SendString("\r\n", SERIAL_BT);
    }
    // 错误命令
    else
        Serial_SendString("Error Command\r\n", SERIAL_BT);
}

// 计时
void TIM3_IRQHandler(void)
{
    // 时间计时
    time.sec++;
    if (time.sec >= 60)
    {
        time.sec = 0;
        time.min++;
    }
    if (time.min >= 60)
    {
        time.min = 0;
        time.hour++;
    }
    if (time.hour >= 24)
    {
        time.hour = 0;
        time.day++;
        Timer_TimestructInit(&timing, TIMING);
    }
    switch (time.month)
    {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        if (time.day > 31)
        {
            time.day = 1;
            time.month++;
        }
        break;
    case 4:
    case 6:
    case 9:
    case 11:
        if (time.day > 30)
        {
            time.day = 1;
            time.month++;
        }
    case 2:
        if (time.year % 4 == 0 && time.year % 100 != 0 || time.year % 400 == 0)
        {
            if (time.day > 29)
            {
                time.day = 1;
                time.month++;
            }
        }
        else
        {
            if (time.day > 28)
            {
                time.day = 1;
                time.month++;
            }
            break;
        }
    default:
        time.month = 1;
        break;
    }
    if (time.month > 12)
    {
        time.month = 1;
        time.year++;
    }

    // 学习时间计时
    if (BodyDetect_Flag == 1)
        timing.sec++;
    if (timing.sec >= 60)
    {
        timing.sec = 0;
        timing.min++;
    }
    if (timing.min >= 60)
    {
        timing.min = 0;
        timing.hour++;
    }

    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
}

/**
 * @brief 传感器刷新定时中断
 * 
 */
void TIM4_IRQHandler(void)
{
    if (IsValue >= IS_EDGE_VALUE)
        BodyDetect_Flag = 1;
    else
        BodyDetect_Flag = 0;

    if (LsValue >= LS_EDGE_VALUE)
        Light_Flag = 1;
    else
        Light_Flag = 0;

    switch (Mode)
    {
    case AUTO:
        Brightness = 100 * (LsValue - LS_EDGE_VALUE) / (LS_MAX_VALUE - LS_EDGE_VALUE);
        if (BodyDetect_Flag == 1 && Light_Flag == 1)
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

/**
 * @brief 蓝牙串口接收中断函数，数据包包头为'@'，包尾为"\r\n"，即回车
 *
 */
void USART1_IRQHandler(void)
{
    static uint8_t RxState = 0;
    static uint8_t pRxPacket = 0;
    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
    {
        uint8_t RxData = USART_ReceiveData(USART1);

        switch (RxState)
        {
        case 0:
            if (RxData == '@')
            {
                // Flag_BTNE = 0;
                pRxPacket = 0;
                RxState = 1;
            }
            break;
        case 1:
            if (RxData == '\r')
            {
                RxState = 2;
            }
            else
            {
                BT_RxPacket[pRxPacket] = RxData;
                pRxPacket++;
            }
            break;
        case 2:
            if (RxData == '\n')
            {
                RxState = 0;
                BT_RxPacket[pRxPacket] = '\0';
                Flag_BTNE = 1;
                Serial_CmdHandler(BT_RxPacket);
            }
            break;
        default:
            break;
        }

        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}
