#ifndef __SERIAL_H
#define __SERIAL_H
#include "stm32f10x.h"
#include <stdio.h>

extern uint8_t Serial_RxFlag;
extern char Serial_RxPacket[];

void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *array, uint16_t length);
void Serial_SendString(char *str);
void Serial_SendNumber(uint32_t num);
//void Serial_SendPacket(void);
//uint8_t Serial_GetRxFlag(void);


#endif
