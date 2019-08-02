#include "MPU6050_UART.h"

MPU6050_Data MPU6050_A;//加速度
MPU6050_Data MPU6050_W;//角速度
MPU6050_Data MPU6050_R;//角度

uint8_t First_Byte_flag_MPU6050=1; //首字节标志
uint8_t RX_Finish=0;

char Command_1[]={0xff,0xaa,0x52};//使Z轴角度归零
char Command_2[]={0xff,0xaa,0x67};//加速度效准
char Command_3[]={0xff,0xaa,0x64};//9600
char Command_4[]={0xff,0xaa,0x63};//115200

uint8_t MPU6050_RXIndex = 0;//当前接收字节数
uint8_t MPU6050_RXBuffer[MPU6050_RX_LEN] = {0};//接收缓冲

void MPU6050_Init()
{
//串口配置为波特率9600(20Hz) ||115200(100HZ)，
//8位数据，无校验(None)，1位停止位，无流控。
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTC, PIN2, FUNMUX0_UART2_RXD, 1);	//GPIOC.2配置为UART2输入引脚
	PORT_Init(PORTC, PIN1, FUNMUX1_UART2_TXD, 0);	//GPIOC.1配置为UART2输出引脚
 	
 	UART_initStruct.Baudrate = 9600;
	UART_initStruct.DataBits = UART_DATA_8BIT;
	UART_initStruct.Parity = UART_PARITY_NONE;
	UART_initStruct.StopBits = UART_STOP_1BIT;
	UART_initStruct.RXThreshold = 1;
	UART_initStruct.RXThresholdIEn = 1;
 	UART_Init(UART2, &UART_initStruct);
	UART_Open(UART2);
	MPU6050_SendCommand(Command_1,3);//使Z轴角度归零
	MPU6050_SendCommand(Command_2,3);//加速度效准
	printf("MPU6050 Init [OK]!\r\n");
//	MPU6050_SendCommand(Command_3,3);//9600波特率
}
void MPU6050_SendCommand(char Data[], uint32_t Len)
{
	uint8_t i;
	for(i=0;i<Len;i++)
	{
	UART_WriteByte(UART2, Data[i]);
	while(UART_IsTXBusy(UART2)==1);
	}
}
void Clear_Data()
{
	uint8_t i;
	for(i=0;i<MPU6050_RX_LEN;i++)
	MPU6050_RXBuffer[i]=0;
}
void UART2_Handler(void)//中断函数
{
	uint32_t chr=0;
	if(UART_INTRXThresholdStat(UART2) || UART_INTTimeoutStat(UART2))
	{
		while(UART_IsRXFIFOEmpty(UART2) == 0)
		{
			if(UART_ReadByte(UART2, &chr) == 0)
			{
				if((chr==MPU6050_StartByte)||(First_Byte_flag_MPU6050==0))
				{
					First_Byte_flag_MPU6050=0;//首字节已识别
				if(MPU6050_RXIndex <=(MPU6050_RX_LEN-2))
				{
					MPU6050_RXBuffer[MPU6050_RXIndex] = (uint8_t)chr;
					MPU6050_RXIndex++;
				}
				else if(MPU6050_RXIndex==(MPU6050_RX_LEN-1))
				{
					MPU6050_RXBuffer[MPU6050_RX_LEN-1]=chr;
					if(MPU6050_RXBuffer[0]==MPU6050_StartByte)
					{
						RX_Finish=1;
						First_Byte_flag_MPU6050=1;//准备再次识别首字节
					  MPU6050_RXIndex=0; //完成一帧（10字节）数据接收，准备下一次接收
					}
					else
					{
					  First_Byte_flag_MPU6050=1;//准备再次识别首字节
					  MPU6050_RXIndex=0; //接收失败，准备下一次接收
						RX_Finish=0;
					}
				}
			}
			}
		}
	}
}
void MPU6050_UART_Handle()
{
	uint8_t i;
	uint8_t Sum=0;
	for(i=0;i<(MPU6050_RX_LEN-1);i++)Sum=Sum+MPU6050_RXBuffer[i];
	if((RX_Finish==1)&&(MPU6050_RXBuffer[10]==Sum))
	{
		NVIC_DisableIRQ(UART2_IRQn);//从UART_RXBuffer读取数据过程中要关闭中断，防止读写混乱
		RX_Finish=0;
		//printf("MPU6050 RX Success!\r\n");
	switch(MPU6050_RXBuffer[1])
						{
							case A_bit:
								MPU6050_A.X=((short)(MPU6050_RXBuffer[3]<<8|MPU6050_RXBuffer[2]))/32768.0*16;
							  MPU6050_A.Y=((short)(MPU6050_RXBuffer[5]<<8|MPU6050_RXBuffer[4]))/32768.0*16;
							  MPU6050_A.Z=((short)(MPU6050_RXBuffer[7]<<8|MPU6050_RXBuffer[6]))/32768.0*16;
							  MPU6050_A.Temp=((short)(MPU6050_RXBuffer[9]<<8|MPU6050_RXBuffer[8]))/340.0+36.25;
//							printf("AAAAA RX Success!\r\n");
//						  printf("XXX%.3f\r\n",MPU6050_A.X);
//		          printf("YYY%.3f\r\n",MPU6050_A.Y);
//		          printf("ZZZ%.3f\r\n",MPU6050_A.Z);
//							printf("Temp%.3f\r\n",MPU6050_A.Temp);
							  Clear_Data();
							  NVIC_EnableIRQ(UART2_IRQn);
							  break;
							case W_bit:
								MPU6050_W.X=((short)(MPU6050_RXBuffer[3]<<8|MPU6050_RXBuffer[2]))/32768.0*2000;
							  MPU6050_W.Y=((short)(MPU6050_RXBuffer[5]<<8|MPU6050_RXBuffer[4]))/32768.0*2000;
							  MPU6050_W.Z=((short)(MPU6050_RXBuffer[7]<<8|MPU6050_RXBuffer[6]))/32768.0*2000;
							  MPU6050_W.Temp=((short)(MPU6050_RXBuffer[9]<<8|MPU6050_RXBuffer[8]))/340.0+36.25;
//							  printf("WWWWW RX Success!\r\n");
//						    printf("XXX%.3f\r\n",MPU6050_W.X);
//		            printf("YYY%.3f\r\n",MPU6050_W.Y);
//		            printf("ZZZ%.3f\r\n",MPU6050_W.Z);
//							  printf("Temp%.3f\r\n",MPU6050_W.Temp);
							  Clear_Data();
							  NVIC_EnableIRQ(UART2_IRQn);
							  break;
							case Roll_bit:
								MPU6050_R.X=((short)(MPU6050_RXBuffer[3]<<8|MPU6050_RXBuffer[2]))/32768.0*180;
							  MPU6050_R.Y=((short)(MPU6050_RXBuffer[5]<<8|MPU6050_RXBuffer[4]))/32768.0*180;
							  MPU6050_R.Z=((short)(MPU6050_RXBuffer[7]<<8|MPU6050_RXBuffer[6]))/32768.0*180;
								MPU6050_R.Temp=((short)(MPU6050_RXBuffer[9]<<8|MPU6050_RXBuffer[8]))/340.0+36.25;
//								printf("RRRRR RX Success!\r\n");
//						    printf("XXX%.3f\r\n",MPU6050_R.X);
//		            printf("YYY%.3f\r\n",MPU6050_R.Y);
//		            printf("ZZZ%.3f\r\n",MPU6050_R.Z);
//							  printf("Temp%.3f\r\n",MPU6050_R.Temp);
							  Clear_Data();
							  NVIC_EnableIRQ(UART2_IRQn);
							  break;
							default:break;
						}
	}
}
