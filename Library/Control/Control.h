#ifndef _CONTROL_H_
#define _CONTROL_H_

#include "swm320.h"

#define Auto 1 //增稳模式
#define Manual 0 //手动模式

#define MAX_Auto_Channel_Value 1800
#define MIN_Auto_Channel_Value 1700

#define MAX_Manual_Channel_Value 500
#define MIN_Manual_Channel_Value 300

#define MAX_Ch1_Value 0x06A8
#define MID_Ch1_Value 0x0400
#define MIN_Ch1_Value 0x0154

#define MAX_Ch2_Value 0x0D48
#define MID_Ch2_Value 0x0800
#define MIN_Ch2_Value 0x0154

#define MAX_Ch3_Value 0x06A8
#define MIN_Ch3_Value 0x0154

#define MAX_Ch4_Value 0x06A8
#define MID_Ch4_Value 0x0400
#define MIN_Ch4_Value 0x0154


void ESC_Servo_Init(void);
void Control_Handle(uint8_t Mode);
uint8_t is_Auto_Manual(uint16_t Ch);

#endif

