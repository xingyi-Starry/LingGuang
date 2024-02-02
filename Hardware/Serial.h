#ifndef __SERIAL_H
#define __SERIAL_H

#define SERIAL_BT USART1
#define SERIAL_VOICE USART2

void Serial_Init(void);
void Serial_SendByte(uint8_t Byte, USART_TypeDef *Serial);
void Serial_SendArray(uint8_t *array, uint16_t length, USART_TypeDef *Serial);
void Serial_SendString(char *str, USART_TypeDef *Serial);
void Serial_SendNumber(uint32_t num, USART_TypeDef *Serial);
uint8_t Serial_RefreshCmdCache(void);
//void Serial_SendPacket(void);
//uint8_t Serial_GetRxFlag(void);

extern char BT_RxPacket[30];
extern char Voice_RxPacket[30];
extern uint8_t Flag_BTNE;
extern uint8_t Flag_VoiceNE;

#endif
