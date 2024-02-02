#ifndef __TIMER_H
#define __TIMER_H

typedef struct 
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
} Time;

typedef enum
{
    DATE = 0x00,
    TIMING = 0x01
} TIME_MODE;

void Timer_TimestructInit(Time *time, TIME_MODE mode);
uint8_t Timer_SetTime(Time *time, char *src);
void Timer_Init(void);

extern uint8_t BodyDetect_Flag;
extern uint8_t Light_Flag;
extern uint8_t Brightness;
extern Time time;
extern Time timing;
#endif
