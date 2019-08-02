#ifndef _SBUS_H_
#define _SBUS_H_

#include "swm320.h"

#define SBUS_RX_LEN	 25 //25字节

#define StartByte 0x0f
#define EndByte 0x00

typedef struct{//25字节SBUS存储结构体
	uint8_t Start;
	uint16_t Ch1;
	uint16_t Ch2;
	uint16_t Ch3;
	uint16_t Ch4;
	uint16_t Ch5;
	uint16_t Ch6;
	uint16_t Ch7;
	uint16_t Ch8;
	uint16_t Ch9;
	uint16_t Ch10;
	uint16_t Ch11;
	uint16_t Ch12;
	uint16_t Ch13;
	uint16_t Ch14;
	uint16_t Ch15;
	uint16_t Ch16;
	uint8_t Flag;
	uint8_t End;
}SBUS_Buffer;
void SBUS_Init(void);
void SBUS_Handle(void);
#endif
