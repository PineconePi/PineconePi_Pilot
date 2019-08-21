#ifndef _MPU6050_UART_H_
#define _MPU6050_UART_H_

#include "swm320.h"

#define MPU6050_RX_LEN	 11 //11�ֽ�
#define MPU6050_StartByte 0x55
#define A_bit 0x51 //���ٶȱ�־
#define W_bit 0x52 //���ٶȱ�־
#define Roll_bit 0x53 //�Ƕȱ�־
typedef struct{
	uint8_t Start;
	uint8_t Flag;//���ٶ�/���ٶ�/�Ƕ�
	float X;
	float Y;
	float Z;
	float Temp;
	uint8_t Sum;
}MPU6050_Data;

void MPU6050_Init(void);
void MPU6050_SendCommand(char Data[], uint32_t Len);
void MPU6050_UART_Handle(void);
#endif


