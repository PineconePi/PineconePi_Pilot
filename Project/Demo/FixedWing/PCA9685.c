/**
  ******************************************************************************
  * @file    PCA9685.c
  * @author  PineconePi (xdd_core)
  * @version V1.0.0
  * @date    14-JULY-2019
  * @brief  This file use to device PCA9685。
  * @License:GNU General Public License v3.0         
  ******************************************************************************
  * @attention
  * FOSC should PLL to 120Mhz.
  * GPIOC.6——>I2C0 SCL
  * GPIOC.5——>I2C0 SDA
  * 
  * 
  * 
  *
  * 
  ******************************************************************************
	**/
#include "PCA9685.h"
#include "math.h"

uint8_t Prescale;//频率设置寄存器输入量

//========================================================================
// 函数: void I2C0_Init()
// 描述: 配置松果派ONE为IIC主机(IIC0)
// 参数: 
//  	 
// 返回: None.
// 版本: VER1.0.0
// 日期: 2019-7-14
// 备注: GPIOC.6配置为I2C0 SCL引脚
//       GPIOC.5配置为I2C0 SDA引脚
//========================================================================
void I2C0_Init()
{
	I2C_InitStructure I2C_initStruct;
	
	PORT_Init(PORTC, PIN6, FUNMUX0_I2C0_SCL, 1);	//GPIOC.6配置为I2C0 SCL引脚
	PORT->PORTC_PULLU |= (1 << PIN6);				//必须使能上拉，用于模拟开漏
	PORT_Init(PORTC, PIN5, FUNMUX1_I2C0_SDA, 1);	//GPIOC.5配置为I2C0 SDA引脚
	PORT->PORTC_PULLU |= (1 << PIN5);				//必须使能上拉，用于模拟开漏
	
	I2C_initStruct.Master = 1;
	I2C_initStruct.Addr7b = 1;
	I2C_initStruct.MstClk = 10000;
	I2C_initStruct.MstIEn = 0;
	I2C_Init(I2C0, &I2C_initStruct);
	
	I2C_Open(I2C0);
}
//========================================================================
// 函数: void PCA9685_Write(uint8_t Addr,uint8_t Dat)
// 描述: 向PCA9685的Addr地址写入一个字节Dat数据
// 参数: Addr:写入地址；Dat：要写入的数据
//  	 
// 返回: None.
// 版本: VER1.0.0
// 日期: 2019-7-14
// 备注: 
// 
//========================================================================
void PCA9685_Write(uint8_t Addr,uint8_t Dat)
{
	  uint32_t i;
	  uint8_t IIC0_TX_Data[2];
	
	  IIC0_TX_Data[0] = Addr;//将数据存入发射缓冲区
	  IIC0_TX_Data[1] = Dat;
	//发送Start信号和器件地址
		I2C0->MSTDAT = (PCA9685_ADDR << 1) | 0; 
		I2C0->MSTCMD = (1 << I2C_MSTCMD_STA_Pos) |		
					   (1 << I2C_MSTCMD_WR_Pos);			//发送起始位和从机地址
		while(I2C0->MSTCMD & I2C_MSTCMD_TIP_Msk) ;			//等待发送完成
		if(I2C0->MSTCMD & I2C_MSTCMD_RXACK_Msk)
		{
			//printf("Slave_NACK for ADDR\r\n");
			goto nextloop;
		}
		else
		{
			//printf("Slave_ACK for ADDR\r\n");
		}
		//发送发送寄存器地址和要写入的数据
				for(i = 0; i < 2; i++)
		{
			
			I2C0->MSTDAT = IIC0_TX_Data[i];
			I2C0->MSTCMD = (1 << I2C_MSTCMD_WR_Pos);
			while(I2C0->MSTCMD & I2C_MSTCMD_TIP_Msk) ;		//等待发送完成
			if(I2C0->MSTCMD & I2C_MSTCMD_RXACK_Msk)
			{
			//	printf("Slave send NACK for data\r\n");
				goto nextloop;
			}
			else
			{
			//	printf("Slave send ACK for data\r\n");
			}
		}
		//等待发送完成，STOP
		I2C0->MSTCMD = (1 << I2C_MSTCMD_STO_Pos);
		while(I2C0->MSTCMD & I2C_MSTCMD_TIP_Msk) ;			//等待发送完成
		
		nextloop: //收到NACK信号，主机执行STOP（异常处理）
		I2C0->MSTCMD = (1 << I2C_MSTCMD_STO_Pos);
		while(I2C0->MSTCMD & I2C_MSTCMD_TIP_Msk) ;			//等待发送完成
		for(i = 0; i < delay_contant; i++) ;
}
//========================================================================
// 函数: uint8_t PCA9685_Read(uint8_t Addr)
// 描述: 向PCA9685的Addr地址读取一个字节数据
// 参数: Addr:写入地址；
//  	 
// 返回: 一个字节数据.
// 版本: VER1.0.0
// 日期: 2019-7-14
// 备注: 
// 
//========================================================================
uint8_t PCA9685_Read(uint8_t Addr)
{ 
	uint8_t RX_Data;
	uint32_t i;
 		//发送Start信号和器件地址
		I2C0->MSTDAT = (PCA9685_ADDR << 1) | 0;
		I2C0->MSTCMD = (1 << I2C_MSTCMD_STA_Pos) |		
					   (1 << I2C_MSTCMD_WR_Pos);			//发送起始位和从机地址
		while(I2C0->MSTCMD & I2C_MSTCMD_TIP_Msk) ;			//等待发送完成
		if(I2C0->MSTCMD & I2C_MSTCMD_RXACK_Msk)
		{
		//	printf("Slave send NACK for address\r\n");
			goto nextloop;
		}
		//发送寄存器地址
			I2C0->MSTDAT = Addr;
			I2C0->MSTCMD = (1 << I2C_MSTCMD_WR_Pos);
			while(I2C0->MSTCMD & I2C_MSTCMD_TIP_Msk) ;		//等待发送完成
			if(I2C0->MSTCMD & I2C_MSTCMD_RXACK_Msk)
			{
		//		printf("Slave send NACK for data\r\n");
				goto nextloop;
			}
		//再次发送Start信号和器件地址
		I2C0->MSTDAT = (PCA9685_ADDR << 1) | 1;
		I2C0->MSTCMD = (1 << I2C_MSTCMD_STA_Pos) |
					   (1 << I2C_MSTCMD_WR_Pos);			//发送Restart
		while(I2C0->MSTCMD & I2C_MSTCMD_TIP_Msk) ;			//等待发送完成
		if(I2C0->MSTCMD & I2C_MSTCMD_RXACK_Msk)
		{
	//		printf("Slave send NACK for address\r\n");
			goto nextloop;
		}
		

		//读取一个字节数据后STOP
		I2C0->MSTCMD = (1 << I2C_MSTCMD_RD_Pos)  |
					   (1 << I2C_MSTCMD_ACK_Pos) |
					   (1 << I2C_MSTCMD_STO_Pos);		//读取数据、发送NACK响应，完成后发送STOP
		while(I2C0->MSTCMD & I2C_MSTCMD_TIP_Msk) ;		//等待接收完成
		RX_Data = I2C0->MSTDAT;
		
		nextloop: //收到NACK信号，主机执行STOP（异常处理）
		I2C0->MSTCMD = (1 << I2C_MSTCMD_STO_Pos);
		while(I2C0->MSTCMD & I2C_MSTCMD_TIP_Msk) ;			//等待发送完成
		for(i = 0; i < delay_contant; i++) ;
		
		
		return RX_Data;
}
//========================================================================
// 函数: void PCA9685_Set_Freq(uint16_t Freq)
// 描述: 设置PCA9685的频率
// 参数: Freq:频率（范围：24(0x03)-1526(0xff)）；
//  	 
// 返回: None.
// 版本: VER1.0.0
// 日期: 2019-7-14
// 备注: 
// 
//========================================================================
void PCA9685_Set_Freq(uint16_t Freq)
{
	uint16_t i;
	float Prescale_Value;
	Prescale_Value = FOSC_MAIN/(4096*Freq*contant);//过程量浮点型
	Prescale = (uint8_t)floor(Prescale_Value+0.5)-1;//向下取整（四舍五入）
//printf("freq:%d\r\n",Prescale);
	PCA9685_Write(0x00,0x11);//进入休眠模式
	for(i = 0; i < 2000; i++) ;
	PCA9685_Write(0xFE,Prescale);//设置频率
	for(i = 0; i < 2000; i++) ;
	PCA9685_Write(0x00,0x01);//退出休眠模式
	for(i = 0; i < 2000; i++) ;
//	PCA9685_Write(0x00,0x81);//复位
//	for(i = 0; i<2000; i++) ;
}
//========================================================================
// 函数: void PCA9685_Set_PWM(uint8_t Ch,uint16_t Value_On,uint16_t Value_Off)
// 描述: 设置PCA9685的Ch通道开关阈值
// 参数: Ch:通道数（范围：0-15）；Value_On：开启阈值（范围：0-4096）
//  	   Value_Off:关闭阈值(范围：0-4096)
// 返回: None.
// 版本: VER1.0.0
// 日期: 2019-7-14
// 备注: 
//      当发现 ACK == LEDX_ON_H[3:0]:LEDX_ON_L 时，X通道输出高电平；
//      当发现 ACK == LEDX_OFF_H[3:0]:LEDX_OFF_L 时，X通道输出低电平。
//========================================================================
void PCA9685_Set_PWM(uint8_t Ch,uint16_t Value_On,uint16_t Value_Off)
{
	uint8_t Value_On_L;//高电平低八位
	uint8_t Value_On_H;//高电平八位
  uint8_t Value_Off_L;//低电平低八位
	uint8_t Value_Off_H;//低电平八位
	
	Value_On_H = (uint8_t)(Value_On>>8);//高电平数据拆分
	Value_On_L = (uint8_t)(Value_On&0x00ff);//高电平数据拆分
	Value_Off_H = (uint8_t)(Value_Off>>8);//低电平数据拆分
	Value_Off_L = (uint8_t)(Value_Off&0x00ff);//低电平数据拆分
	
	PCA9685_Write(0x06+Ch*4,Value_On_L);//设置ON(高电平)阈值低八位
	PCA9685_Write(0x06+Ch*4+1,Value_On_H);//设置ON(高电平)阈值高八位
	PCA9685_Write(0x06+Ch*4+2,Value_Off_L);//设置OFF(低电平)阈值低八位
	PCA9685_Write(0x06+Ch*4+3,Value_Off_H);//设置OFF(低电平)阈值高八位
}
//========================================================================
// 函数: void PCA9685_Servo_Control(uint8_t Ch,uint8_t Angle)
// 描述: 舵机控制函数
// 参数: Ch:通道数（范围：0-15）；Angle:舵机角度（范围：0-180）
//  	 
// 返回: None.
// 版本: VER1.0.0
// 日期: 2019-7-14
// 备注: 
// 
//========================================================================
void PCA9685_Servo_Angle_Control(uint8_t Ch,uint8_t Angle)
{
	//0.5(102.4)-2.5(512)
	double Value;//计算过程值用浮点型保存
	uint16_t PWM_Value;
	Value = Angle;
	Value = (Value/180*2+Servo_Range_Base)/20*4096;
	Value = floor(Value+0.5);
	PWM_Value = (uint16_t)Value;
	PCA9685_Set_PWM(Ch,0,PWM_Value);
}
//========================================================================
// 函数: void PCA9685_Servo_Range_Control(uint8_t Ch,uint16_t Range)
// 描述: 舵机控制函数
// 参数: Ch:通道数（范围：0-15）；Range:舵机范围（范围：0-1000|45°-135°）
//  	 
// 返回: None.
// 版本: VER1.0.0
// 日期: 2019-7-14
// 备注: 
// 
//========================================================================
void PCA9685_Servo_Range_Control(uint8_t Ch,uint16_t Range)
{
	//1.0(210)-2.0(410) //200
	double Value;//计算过程值用浮点型保存
	uint16_t PWM_Value;
	Value = Range;
	Value = (Value+1050)/5;
	Value = floor(Value+0.5);
	PWM_Value = (uint16_t)Value;
	PCA9685_Set_PWM(Ch,0,PWM_Value);
}
//========================================================================
// 函数: void Pca9685_ESC_Control(uint8_t Ch,double Range)
// 描述: 电调控制函数
// 参数: Ch:通道数（范围：0-15）；Range:电调油门（范围：0-1000）
//  	 
// 返回: None.
// 版本: VER1.0.0
// 日期: 2019-7-14
// 备注: 
// 
//========================================================================
void Pca9685_ESC_Control(uint8_t Ch,double Range)//电调
{
	uint16_t ESC_Value;
	Range+=1065;
	Range/=5;
	Range = floor(Range+0.5);
	ESC_Value = (uint16_t)Range;
	PCA9685_Set_PWM(Ch,0,ESC_Value);
}
//========================================================================
// 函数: uint8_t PCA9685_Init()
// 描述: PCA9685初始化函数
// 参数: None.
//  	 
// 返回: 初始化结果（0：成功；1：失败）.
// 版本: VER1.0.0
// 日期: 2019-7-14
// 备注: 
// 
//========================================================================
uint8_t PCA9685_Init()
{
	I2C0_Init();//配置IIC0端口与初始化IIC0
	PCA9685_Set_Freq(freq);//设置频率为50HZ(20MS)
	if(Prescale == PCA9685_Read(0xFE))//检查频率设置是否正确
	{
		printf("PCA9685 Init [OK]!\r\n");
		return 0;
	}
	else
	{
		printf("PCA9685 Init [ERROR]!\r\n");
		//此处可添加报警操作 or飞控锁定操作（禁止读接收机）
		return 1;
	}
}
