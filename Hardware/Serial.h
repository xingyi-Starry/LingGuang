#ifndef __SERIAL_H
#define __SERIAL_H

#define SERIAL_BT USART1
#define SERIAL_VOICE USART2

typedef enum
{
    TurnOn_OK = 0x00,
    TurnOff_OK = 0x01,
    BriUP_OK,
    BriDown_OK,
    MaxBrightness,
    MinBrightness,
    AutoMode_OK,
    ManualMode_OK,
    SwitchMode_OK,
    SwitchState_OK,
    LearnTimeSpk,
    ManModeFirst
} HC05MESSAGE_enum;

void Serial_Init(void);
void Serial_SendByte(uint8_t Byte, USART_TypeDef *Serial);
void Serial_SendArray(uint8_t *array, uint16_t length, USART_TypeDef *Serial);
void Serial_SendString(char *str, USART_TypeDef *Serial);
void Serial_SendNumber(uint32_t num, USART_TypeDef *Serial);
void HC05_SendMessage(HC05MESSAGE_enum Message);
void HC05_LearnTimeSpk(uint8_t hour, uint8_t min, uint8_t sec);

extern char BT_RxPacket[30];
extern char Voice_RxPacket[30];

#endif
