#ifndef __SENSOR_H
#define __SENSOR_H

#define LS_EDGE_VALUE 2000 // 光敏临界值
#define IS_EDGE_VALUE 3000 // 红外临界值
#define LS_MAX_VALUE 4000 // 光敏最大值
#define IS_MAX_VALUE 4000 // 红外最大值
#define LsValue Sensor_Value[1] // 光敏传感器数值
#define IsVlaue Sensor_Value[0] // 红外传感器数值

extern uint16_t Sensor_Value[];

void Sensor_Init(void);

#endif
