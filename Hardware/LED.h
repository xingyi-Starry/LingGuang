#ifndef __LED_H
#define __LED_H

#define LOWEST_BRI 100  // 最低亮度对应CCR
#define HIGHEST_BRI 200 // 最高亮度对应CCR

typedef enum
{
    MANUAL,
    AUTO
} LED_MODE;

typedef enum
{
    OFF,
    ON
} LED_STATE;

void LED_Init(void);
void LED_SetBrightness(uint16_t Brightness);
void LED_TurnOff(void);


#endif
