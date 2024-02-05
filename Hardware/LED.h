#ifndef __LED_H
#define __LED_H

#define LOWEST_BRI 100  // 最低亮度对应CCR
#define HIGHEST_BRI 200 // 最高亮度对应CCR

/**
 * @brief LED模式枚举，取值为MANUAL(0)和AUTO(1)
 * 
 */
typedef enum
{
    MANUAL,
    AUTO
} LED_MODE;

/**
 * @brief LED状态枚举，取值为OFF(0)和ON(1)
 * 
 */
typedef enum
{
    OFF,
    ON
} LED_STATE;

void LED_Init(void);
void LED_AutoMode(void);
void LED_ManualMode(void);
void LED_SwitchMode(void);
uint8_t LED_SwitchState(void);
void LED_SetBrightness(uint16_t Brightness);
void LED_TurnOn(void);
void LED_TurnOff(void);
uint8_t LED_BriUp(void);
uint8_t LED_BriDown(void);

extern LED_MODE Mode;
extern LED_STATE State;
extern uint8_t Man_Bri;
#endif
