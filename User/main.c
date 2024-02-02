#include <string.h>
#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "LED.h"
#include "OLED.h"
#include "Key.h"
#include "LED.h"
#include "Timer.h"
#include "Sensor.h"
#include "Serial.h"

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
	OLED_ShowString(4, 1, "00:00:00");
//	OLED_ShowString(4, 1, "Ls:");
//	OLED_ShowString(4, 8, "Is:");

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

		// 接受串口指令
		if (Serial_RxFlag == 1)
		{
			// 输入时间
			if (Serial_RxPacket[4] == '/' && Serial_RxPacket[7] == '/' && Serial_RxPacket[10] == '/' && Serial_RxPacket[13] == ':' && Serial_RxPacket[16] == ':')
			{
				if (Timer_SetTime(&time, Serial_RxPacket) == 1)
					Serial_SendString("Wrong time\r\n", SERIAL_BT);
				else
					Serial_SendString("Time set success\r\n", SERIAL_BT);
			}
			// 切换模式
			else if (strcmp(Serial_RxPacket, "SwitchMode") == 0)
			{
				LED_SwitchMode();
				Serial_SendString("Mode switch successful\r\n", SERIAL_BT);
			}
			// 切换状态
			else if (strcmp(Serial_RxPacket, "SwitchState") == 0)
			{
				if (LED_SwitchState() == 1)
					Serial_SendString("Error: please switch to mannual mode first\r\n", SERIAL_BT);
				else 
					Serial_SendString("State switch successful\r\n", SERIAL_BT);
			}
			// 获取学习时间
			else if (strcmp(Serial_RxPacket, "GetLearningTime") == 0)
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
			Serial_RxFlag = 0;
		}

		OLED_ShowNum(1, 6, Mode, 1);
		OLED_ShowNum(1, 12, State, 1);
//		OLED_ShowNum(4, 4, LsValue, 4);
//		OLED_ShowNum(4, 11, IsValue, 4);
		// 显示时间
		OLED_ShowNum(2, 6, time.year, 4);
		OLED_ShowNum(2, 11, time.month, 2);
		OLED_ShowNum(2, 14, time.day, 2);
		OLED_ShowNum(3, 6, time.hour, 2);
		OLED_ShowNum(3, 9, time.min, 2);
		OLED_ShowNum(3, 12, time.sec, 2);

		// 计时显示
		OLED_ShowNum(4, 1, timing.hour, 2);
		OLED_ShowNum(4, 4, timing.min, 2);
		OLED_ShowNum(4, 7, timing.sec, 2);
	}
}
