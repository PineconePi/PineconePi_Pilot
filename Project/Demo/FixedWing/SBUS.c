#include "SBUS.h"

SBUS_Buffer SBUS;
uint8_t First_Byte_flag_SBUS=1; //首字节标志
uint8_t SBUS_RX_Finish=0;
uint8_t SBUS_RXIndex = 0;//当前接收字节数
uint8_t SBUS_RXBuffer[SBUS_RX_LEN] = {0};//接收缓冲
void SBUS_Init()
{
	//串口配置为波特率100kbps，
//8位数据，偶校验(even)，2位停止位，无流控。
//25字节
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTC, PIN4, FUNMUX0_UART1_RXD, 1);	//GPIOC.4配置为UART1输入引脚
	PORT_Init(PORTC, PIN3, FUNMUX1_UART1_TXD, 0);	//GPIOC.3配置为UART1输出引脚
 	
 	UART_initStruct.Baudrate = 100000;
	UART_initStruct.DataBits = UART_DATA_8BIT;
	UART_initStruct.Parity = UART_PARITY_EVEN;
	UART_initStruct.StopBits = UART_STOP_2BIT;
	UART_initStruct.RXThreshold = 1;
	UART_initStruct.RXThresholdIEn = 1;
 	UART_Init(UART1, &UART_initStruct);
	UART_Open(UART1);
	printf("SBUS Init [OK]!\r\n");
}
void UART1_Handler(void)//中断函数
{
	uint32_t chr;
	if(UART_INTRXThresholdStat(UART1) || UART_INTTimeoutStat(UART1))
	{
		while(UART_IsRXFIFOEmpty(UART1) == 0)
		{
			if(UART_ReadByte(UART1, &chr) == 0)
			{
				if((chr==0x0f)||(First_Byte_flag_SBUS==0))
				{
					First_Byte_flag_SBUS=0;//首字节已识别
				if(SBUS_RXIndex < (SBUS_RX_LEN-2))
				{
					SBUS_RXBuffer[SBUS_RXIndex] = chr;
					SBUS_RXIndex++;
				}
				else if(SBUS_RXIndex < (SBUS_RX_LEN-1))
				{
					SBUS_RXBuffer[SBUS_RX_LEN-1] = chr;
					First_Byte_flag_SBUS=1;//准备再次识别首字节
					SBUS_RXIndex=0; //完成一帧（25字节）数据接收，准备下一次接收
					if((SBUS_RXBuffer[0]==StartByte)&&(SBUS_RXBuffer[24]==EndByte))
					{
	//					printf("SBUS RX Success!");
						SBUS_RX_Finish=1;//接收成功
					}
					else
					{
						SBUS_RX_Finish=0;//接收失败
					  First_Byte_flag_SBUS=1;//准备再次识别首字节
					  SBUS_RXIndex=0; //接收失败，准备下一次接收
//						printf("SBUS RX Error!");
					}
				}
			}
			}
		}
	}
}
void SBUS_Handle()
{
	if(SBUS_RX_Finish==1)
	{
		SBUS_RX_Finish=0;//准备下一次接收
		NVIC_DisableIRQ(UART1_IRQn);//从UART_RXBuffer读取数据过程中要关闭中断，防止读写混乱
		SBUS.Start=SBUS_RXBuffer[0];
		SBUS.Ch1=((uint16_t)SBUS_RXBuffer[1])|((uint16_t)((SBUS_RXBuffer[2]&0x07)<<8));
		SBUS.Ch2=((uint16_t)((SBUS_RXBuffer[2]&0xf8)>>3))|(((uint16_t)(SBUS_RXBuffer[3]&0x3f))<<6);
		SBUS.Ch3=((uint16_t)((SBUS_RXBuffer[3]&0xc0)>>6))|((((uint16_t)SBUS_RXBuffer[4])<<2))|(((uint16_t)(SBUS_RXBuffer[5]&0x01))<<10);
		SBUS.Ch4=((uint16_t)((SBUS_RXBuffer[5]&0xfe)>>1))|(((uint16_t)(SBUS_RXBuffer[6]&0x0f))<<7);
		SBUS.Ch5=((uint16_t)((SBUS_RXBuffer[6]&0xf0)>>4))|(((uint16_t)(SBUS_RXBuffer[7]&0x7f))<<4);
		SBUS.Ch6=((uint16_t)((SBUS_RXBuffer[7]&0x80)>>7))|(((uint16_t)SBUS_RXBuffer[8])<<1)|(((uint16_t)(SBUS_RXBuffer[9]&0x03))<<9);
		SBUS.Ch7=((uint16_t)((SBUS_RXBuffer[9]&0xfc)>>2))|(((uint16_t)(SBUS_RXBuffer[10]&0x1f))<<6);
		SBUS.Ch8=((uint16_t)((SBUS_RXBuffer[10]&0xe0)>>5))|(((uint16_t)(SBUS_RXBuffer[11]))<<3);
		SBUS.Ch9=((uint16_t)SBUS_RXBuffer[12])|(((uint16_t)(SBUS_RXBuffer[13]&0x07))<<8);
		SBUS.Ch10=((uint16_t)((SBUS_RXBuffer[13]&0xf8)>>3))|(((uint16_t)(SBUS_RXBuffer[14]&0x3f))<<5);
		SBUS.Ch11=((uint16_t)((SBUS_RXBuffer[14]&0xc0)>>6))|(((uint16_t)SBUS_RXBuffer[15])<<2)|(((uint16_t)(SBUS_RXBuffer[16]&0x01))<<10);
		SBUS.Ch12=((uint16_t)((SBUS_RXBuffer[16]&0xfe)>>1))|(((uint16_t)(SBUS_RXBuffer[17]&0x0f))<<7);
		SBUS.Ch13=((uint16_t)((SBUS_RXBuffer[17]&0xf0)>>4))|(((uint16_t)(SBUS_RXBuffer[18]&0x7f))<<4);
		SBUS.Ch14=((uint16_t)((SBUS_RXBuffer[18]&0x80)>>7))|(((uint16_t)SBUS_RXBuffer[19])<<1)|(((uint16_t)(SBUS_RXBuffer[20]&0x03))<<9);
		SBUS.Ch15=((uint16_t)((SBUS_RXBuffer[20]&0xfc)>>2))|(((uint16_t)(SBUS_RXBuffer[21]&0x1f))<<6);
		SBUS.Ch16=((uint16_t)((SBUS_RXBuffer[21]&0xe0)>>5))|(((uint16_t)SBUS_RXBuffer[22])<<3);
		SBUS.Flag=SBUS_RXBuffer[23];
	  SBUS.End=SBUS_RXBuffer[24];
		NVIC_EnableIRQ(UART1_IRQn);
	}
}
