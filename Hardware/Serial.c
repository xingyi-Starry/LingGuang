#include <string.h>
#include "stm32f10x.h"
#include "Serial.h"
#include "Timer.h"

char BT_RxPacket[30];
char Voice_RxPacket[30];

void Serial_Init(void)
{
    /*开启外设时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    /*GPIO初始化*/
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /*USART初始化*/
    USART_InitTypeDef USART_InitStructure;
    USART_StructInit(&USART_InitStructure);
    USART_InitStructure.USART_BaudRate = 9600;                                      // 波特率
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 硬件流控制
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStructure.USART_Parity = USART_Parity_No;         // 校验
    USART_InitStructure.USART_StopBits = USART_StopBits_1;      // 停止位长度
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; // 字长(数据长度+校验位长度)
    USART_Init(USART1, &USART_InitStructure);
    USART_Init(USART2, &USART_InitStructure);

    /*开启USART中断通道*/
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

    /*NVIC初始化*/
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);

    /*使能USART*/
    USART_Cmd(USART1, ENABLE);
    USART_Cmd(USART2, ENABLE);
}

void Serial_SendByte(uint8_t Byte, USART_TypeDef *Serial)
{
    USART_SendData(Serial, Byte);
    /*等待数据写入移位寄存器*/
    while (USART_GetFlagStatus(Serial, USART_FLAG_TXE) == RESET)
        ; // TXE在数据转移到移位寄存器后置1(SET), 在对寄存器进行写操作后自动清零(RESET), 因此不用手动清零
}

void Serial_SendArray(uint8_t *array, uint16_t length, USART_TypeDef *Serial)
{
    uint16_t i;
    for (i = 0; i < length; i++)
    {
        Serial_SendByte(array[i], Serial);
    }
}

void Serial_SendString(char *str, USART_TypeDef *Serial)
{
    uint8_t i;
    for (i = 0; str[i] != 0; i++)
    {
        Serial_SendByte(str[i], Serial);
    }
}

void Serial_SendNumber(uint32_t num, USART_TypeDef *Serial)
{
    uint32_t bit = 1;
    for (uint32_t temp = num; temp >= 10; bit *= 10)
    {
        temp /= 10;
    }

    for (; bit > 0; bit /= 10)
    {
        Serial_SendByte(num / bit + '0', Serial);
        num %= bit;
    }
}

/**
 * @brief HC05命令
 * 
 * @param Message 命令
 */
void HC05_SendMessage(HC05MESSAGE_enum Message)
{
    Serial_SendByte('@', SERIAL_VOICE);
    Serial_SendByte(Message, SERIAL_VOICE);
    Serial_SendByte('\r', SERIAL_VOICE);
    Serial_SendByte('\n', SERIAL_VOICE);
}

void HC05_LearnTimeSpk(uint8_t hour, uint8_t min, uint8_t sec)
{
    Serial_SendByte('@', SERIAL_VOICE);
    Serial_SendByte(LearnTimeSpk, SERIAL_VOICE);
    Serial_SendByte(hour, SERIAL_VOICE);
    Serial_SendByte(min, SERIAL_VOICE);
    Serial_SendByte(sec, SERIAL_VOICE);
    Serial_SendByte('\r', SERIAL_VOICE);
    Serial_SendByte('\n', SERIAL_VOICE);    
}
