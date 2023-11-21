#include "stm32f10x.h"

char Serial_RxPacket[100];
uint8_t Serial_RxFlag;

void Serial_Init(void)
{
    /*开启外设时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    /*GPIO初始化*/
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /*USART初始化*/
    USART_InitTypeDef USART_InitStructure;
    USART_StructInit(&USART_InitStructure);
    USART_InitStructure.USART_BaudRate = 9600; //波特率
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //硬件流控制
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStructure.USART_Parity = USART_Parity_No; //校验
    USART_InitStructure.USART_StopBits = USART_StopBits_1; //停止位长度
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //字长(数据长度+校验位长度)
    USART_Init(USART1, &USART_InitStructure);

    /*开启USART中断通道*/
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    /*NVIC初始化*/
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);

    /*使能USART*/
    USART_Cmd(USART1, ENABLE);
}

void Serial_SendByte(uint8_t Byte)
{
    USART_SendData(USART1, Byte);
    /*等待数据写入移位寄存器*/
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); //TXE在数据转移到移位寄存器后置1(SET), 在对寄存器进行写操作后自动清零(RESET), 因此不用手动清零
}

void Serial_SendArray(uint8_t *array, uint16_t length)
{
    uint16_t i;
    for (i = 0; i < length; i++)
    {
        Serial_SendByte(array[i]);
    }
}

void Serial_SendString(char *str)
{
    uint8_t i;
    for (i = 0; str[i] != 0; i++)
    {
        Serial_SendByte(str[i]);
    }
}

void Serial_SendNumber(uint32_t num)
{
    uint32_t bit = 1;
    for (uint32_t temp = num; temp >= 10; bit *= 10)
    {
        temp /= 10;
    }

    for (; bit > 0; bit /= 10)
    {
        Serial_SendByte(num / bit + '0');
        num %= bit;
    }
}

/*
uint8_t Serial_GetRxFlag(void)
{
    if (Serial_RxFlag == 1)
    {
        Serial_RxFlag = 0;
        return 1;
    }
    return 0;
}*/

/*接收中断函数*/
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
            if (RxData == '@' && Serial_RxFlag == 0)
            {
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
                Serial_RxPacket[pRxPacket] = RxData;
                pRxPacket++;
            }
            break;
        case 2:
            if (RxData == '\n')
            {
                RxState = 0;
                Serial_RxPacket[pRxPacket] = '\0';
                Serial_RxFlag = 1;
            }
            break;
        default:
            break;
        }

        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}
