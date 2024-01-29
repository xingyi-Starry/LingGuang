#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "LED.h"
#include "OLED.h"
#include "Key.h"
#include "LED.h"
#include "Timer.h"
#include "Sensor.h"
#include "Serial.h"

extern Time time;

int main(void)
{
	/*NVIC优先级分组*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	Key_Init();
	LED_Init();
	OLED_Init();
	Sensor_Init();
	Timer_Init();
	Serial_Init();

	OLED_ShowString(1, 1, "Mode:");
	OLED_ShowString(1, 8, "Sta:");
	OLED_ShowString(2, 1, "Date:0000/00/00");
	OLED_ShowString(3, 1, "Time:00:00:00");
	OLED_ShowString(4, 1, "Ls:");
	OLED_ShowString(4, 8, "Is:");

	while (1)
	{
		/*
		if (key2_State == 1)
		{
			if (Sensor_Value[1] >= 2000 /*&& Sensor_Value[0] <= 3000*)
			{
				OLED_ShowString(1, 11, "ON ");
				PWM_SetBrightness(Sensor_Value[1] / 20 - 100);
			}
			else
			{
				OLED_ShowString(1, 11, "OFF");
				PWM_SetBrightness(0);
			}
		}
		else if (key2_State == 0)
		{
			if (key1_Switch == 0)
			{
				OLED_ShowString(1, 11, "OFF");
				PWM_SetBrightness(0);
			}
			else if (key1_Switch == 1)
			{
				OLED_ShowString(1, 11, "ON ");
				PWM_SetBrightness(100);
			}
		}*/

		if (Serial_RxFlag == 1)
		{

			if (Serial_RxPacket[4] == '/' && Serial_RxPacket[7] == '/' && Serial_RxPacket[10] == '/' && Serial_RxPacket[13] == ':' && Serial_RxPacket[16] == ':')
			{
				time.year = (Serial_RxPacket[0] - '0') * 1000 + (Serial_RxPacket[1] - '0') * 100 + (Serial_RxPacket[2] - '0') * 10 + (Serial_RxPacket[3] - '0');
				time.month = (Serial_RxPacket[5] - '0') * 10 + (Serial_RxPacket[6] - '0');
				time.day = (Serial_RxPacket[8] - '0') * 10 + (Serial_RxPacket[9] - '0');
				time.hour = (Serial_RxPacket[11] - '0') * 10 + (Serial_RxPacket[12] - '0');
				time.min = (Serial_RxPacket[14] - '0') * 10 + (Serial_RxPacket[15] - '0');
				time.sec = (Serial_RxPacket[17] - '0') * 10 + (Serial_RxPacket[18] - '0');
			}
			else
			{
				Serial_SendString("Wrong Command");
			}
			Serial_RxFlag = 0;
		}

		OLED_ShowNum(1, 6, Mode, 1);
		OLED_ShowNum(1, 12, State, 1);
		OLED_ShowNum(4, 4, LsValue, 4);
		OLED_ShowNum(4, 11, IsValue, 4);
		// 显示时间
		OLED_ShowNum(2, 6, time.year, 4);
		OLED_ShowNum(2, 11, time.month, 2);
		OLED_ShowNum(2, 14, time.day, 2);
		OLED_ShowNum(3, 6, time.hour, 2);
		OLED_ShowNum(3, 9, time.min, 2);
		OLED_ShowNum(3, 12, time.sec, 2);
	}
}
