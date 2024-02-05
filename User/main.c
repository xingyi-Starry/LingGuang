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

	// 主循环啥也没了，乐
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
