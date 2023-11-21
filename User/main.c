#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "LED.h"
#include "OLED.h"
#include "Key.h"
#include "PWM.h"
#include "Timer.h"
#include "Sensor.h"

int main(void)
{
	/*NVIC优先级分组*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	Key_Init();
	PWM_Init();
	OLED_Init();
	Sensor_Init();
	Timer_Init();

	OLED_ShowString(1, 1, "State:");
	OLED_ShowString(2, 1, "Switch:");
	OLED_ShowString(3, 1, "Time:00:00:00");
	OLED_ShowString(4, 1, "Ls:");
	OLED_ShowString(4, 8, "Is:");


	while (1)
	{
		if (key2_State == 1)
		{
			if (Sensor_Value[1] >= 3000 && Sensor_Value[0] <= 3000)
			{
				key1_Switch = 1;
			}
			else 
			{
				key1_Switch = 0;
			}
		}
		if (key1_Switch == 0)
		{
			OLED_ShowString(2, 8, "OFF");
			PWM_SetBrightness(0);
		}
		else if (key1_Switch == 1)
		{
			OLED_ShowString(2, 8, "ON ");
			PWM_SetBrightness(100);
		}

		OLED_ShowNum(1, 7, key2_State, 1);
		OLED_ShowNum(4, 4, Sensor_Value[1], 4);
		OLED_ShowNum(4, 11, Sensor_Value[0], 4);
		OLED_ShowNum(3, 6, hour, 2);
		OLED_ShowNum(3, 9, min, 2);
		OLED_ShowNum(3, 12, sec, 2);
	}
}
