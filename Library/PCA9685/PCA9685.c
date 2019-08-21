/**
  ******************************************************************************
  * @file    PCA9685.c
  * @author  PineconePi (xdd_core)
  * @version V1.0.0
  * @date    14-JULY-2019
  * @brief  This file use to device PCA9685��
  * @License:GNU General Public License v3.0         
  ******************************************************************************
  * @attention
  * FOSC should PLL to 120Mhz.
  * GPIOC.6����>I2C0 SCL
  * GPIOC.5����>I2C0 SDA
  * 
  * 
  * 
  *
  * 
  ******************************************************************************
	**/
#include "PCA9685.h"
#include "math.h"

uint8_t Prescale;//Ƶ�����üĴ���������

//========================================================================
// ����: void I2C0_Init()
// ����: �����ɹ���ONEΪIIC����(IIC0)
// ����: 
//  	 
// ����: None.
// �汾: VER1.0.0
// ����: 2019-7-14
// ��ע: GPIOC.6����ΪI2C0 SCL����
//       GPIOC.5����ΪI2C0 SDA����
//========================================================================
void I2C0_Init()
{
	I2C_InitStructure I2C_initStruct;
	
	PORT_Init(PORTC, PIN6, FUNMUX0_I2C0_SCL, 1);	//GPIOC.6����ΪI2C0 SCL����
	PORT->PORTC_PULLU |= (1 << PIN6);				//����ʹ������������ģ�⿪©
	PORT_Init(PORTC, PIN5, FUNMUX1_I2C0_SDA, 1);	//GPIOC.5����ΪI2C0 SDA����
	PORT->PORTC_PULLU |= (1 << PIN5);				//����ʹ������������ģ�⿪©
	
	I2C_initStruct.Master = 1;
	I2C_initStruct.Addr7b = 1;
	I2C_initStruct.MstClk = 10000;
	I2C_initStruct.MstIEn = 0;
	I2C_Init(I2C0, &I2C_initStruct);
	
	I2C_Open(I2C0);
}
//========================================================================
// ����: void PCA9685_Write(uint8_t Addr,uint8_t Dat)
// ����: ��PCA9685��Addr��ַд��һ���ֽ�Dat����
// ����: Addr:д���ַ��Dat��Ҫд�������
//  	 
// ����: None.
// �汾: VER1.0.0
// ����: 2019-7-14
// ��ע: 
// 
//========================================================================
void PCA9685_Write(uint8_t Addr,uint8_t Dat)
{
	  uint32_t i;
	  uint8_t IIC0_TX_Data[2];
	
	  IIC0_TX_Data[0] = Addr;//�����ݴ��뷢�仺����
	  IIC0_TX_Data[1] = Dat;
	//����Start�źź�������ַ
		I2C0->MSTDAT = (PCA9685_ADDR << 1) | 0; 
		I2C0->MSTCMD = (1 << I2C_MSTCMD_STA_Pos) |		
					   (1 << I2C_MSTCMD_WR_Pos);			//������ʼλ�ʹӻ���ַ
		while(I2C0->MSTCMD & I2C_MSTCMD_TIP_Msk) ;			//�ȴ��������
		if(I2C0->MSTCMD & I2C_MSTCMD_RXACK_Msk)
		{
			//printf("Slave_NACK for ADDR\r\n");
			goto nextloop;
		}
		else
		{
			//printf("Slave_ACK for ADDR\r\n");
		}
		//���ͷ��ͼĴ�����ַ��Ҫд�������
				for(i = 0; i < 2; i++)
		{
			
			I2C0->MSTDAT = IIC0_TX_Data[i];
			I2C0->MSTCMD = (1 << I2C_MSTCMD_WR_Pos);
			while(I2C0->MSTCMD & I2C_MSTCMD_TIP_Msk) ;		//�ȴ��������
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
		//�ȴ�������ɣ�STOP
		I2C0->MSTCMD = (1 << I2C_MSTCMD_STO_Pos);
		while(I2C0->MSTCMD & I2C_MSTCMD_TIP_Msk) ;			//�ȴ��������
		
		nextloop: //�յ�NACK�źţ�����ִ��STOP���쳣����
		I2C0->MSTCMD = (1 << I2C_MSTCMD_STO_Pos);
		while(I2C0->MSTCMD & I2C_MSTCMD_TIP_Msk) ;			//�ȴ��������
		for(i = 0; i < delay_contant; i++) ;
}
//========================================================================
// ����: uint8_t PCA9685_Read(uint8_t Addr)
// ����: ��PCA9685��Addr��ַ��ȡһ���ֽ�����
// ����: Addr:д���ַ��
//  	 
// ����: һ���ֽ�����.
// �汾: VER1.0.0
// ����: 2019-7-14
// ��ע: 
// 
//========================================================================
uint8_t PCA9685_Read(uint8_t Addr)
{ 
	uint8_t RX_Data;
	uint32_t i;
 		//����Start�źź�������ַ
		I2C0->MSTDAT = (PCA9685_ADDR << 1) | 0;
		I2C0->MSTCMD = (1 << I2C_MSTCMD_STA_Pos) |		
					   (1 << I2C_MSTCMD_WR_Pos);			//������ʼλ�ʹӻ���ַ
		while(I2C0->MSTCMD & I2C_MSTCMD_TIP_Msk) ;			//�ȴ��������
		if(I2C0->MSTCMD & I2C_MSTCMD_RXACK_Msk)
		{
		//	printf("Slave send NACK for address\r\n");
			goto nextloop;
		}
		//���ͼĴ�����ַ
			I2C0->MSTDAT = Addr;
			I2C0->MSTCMD = (1 << I2C_MSTCMD_WR_Pos);
			while(I2C0->MSTCMD & I2C_MSTCMD_TIP_Msk) ;		//�ȴ��������
			if(I2C0->MSTCMD & I2C_MSTCMD_RXACK_Msk)
			{
		//		printf("Slave send NACK for data\r\n");
				goto nextloop;
			}
		//�ٴη���Start�źź�������ַ
		I2C0->MSTDAT = (PCA9685_ADDR << 1) | 1;
		I2C0->MSTCMD = (1 << I2C_MSTCMD_STA_Pos) |
					   (1 << I2C_MSTCMD_WR_Pos);			//����Restart
		while(I2C0->MSTCMD & I2C_MSTCMD_TIP_Msk) ;			//�ȴ��������
		if(I2C0->MSTCMD & I2C_MSTCMD_RXACK_Msk)
		{
	//		printf("Slave send NACK for address\r\n");
			goto nextloop;
		}
		

		//��ȡһ���ֽ����ݺ�STOP
		I2C0->MSTCMD = (1 << I2C_MSTCMD_RD_Pos)  |
					   (1 << I2C_MSTCMD_ACK_Pos) |
					   (1 << I2C_MSTCMD_STO_Pos);		//��ȡ���ݡ�����NACK��Ӧ����ɺ���STOP
		while(I2C0->MSTCMD & I2C_MSTCMD_TIP_Msk) ;		//�ȴ��������
		RX_Data = I2C0->MSTDAT;
		
		nextloop: //�յ�NACK�źţ�����ִ��STOP���쳣����
		I2C0->MSTCMD = (1 << I2C_MSTCMD_STO_Pos);
		while(I2C0->MSTCMD & I2C_MSTCMD_TIP_Msk) ;			//�ȴ��������
		for(i = 0; i < delay_contant; i++) ;
		
		
		return RX_Data;
}
//========================================================================
// ����: void PCA9685_Set_Freq(uint16_t Freq)
// ����: ����PCA9685��Ƶ��
// ����: Freq:Ƶ�ʣ���Χ��24(0x03)-1526(0xff)����
//  	 
// ����: None.
// �汾: VER1.0.0
// ����: 2019-7-14
// ��ע: 
// 
//========================================================================
void PCA9685_Set_Freq(uint16_t Freq)
{
	uint16_t i;
	float Prescale_Value;
	Prescale_Value = FOSC_MAIN/(4096*Freq*contant);//������������
	Prescale = (uint8_t)floor(Prescale_Value+0.5)-1;//����ȡ�����������룩
//printf("freq:%d\r\n",Prescale);
	PCA9685_Write(0x00,0x11);//��������ģʽ
	for(i = 0; i < 2000; i++) ;
	PCA9685_Write(0xFE,Prescale);//����Ƶ��
	for(i = 0; i < 2000; i++) ;
	PCA9685_Write(0x00,0x01);//�˳�����ģʽ
	for(i = 0; i < 2000; i++) ;
//	PCA9685_Write(0x00,0x81);//��λ
//	for(i = 0; i<2000; i++) ;
}
//========================================================================
// ����: void PCA9685_Set_PWM(uint8_t Ch,uint16_t Value_On,uint16_t Value_Off)
// ����: ����PCA9685��Chͨ��������ֵ
// ����: Ch:ͨ��������Χ��0-15����Value_On��������ֵ����Χ��0-4096��
//  	   Value_Off:�ر���ֵ(��Χ��0-4096)
// ����: None.
// �汾: VER1.0.0
// ����: 2019-7-14
// ��ע: 
//      ������ ACK == LEDX_ON_H[3:0]:LEDX_ON_L ʱ��Xͨ������ߵ�ƽ��
//      ������ ACK == LEDX_OFF_H[3:0]:LEDX_OFF_L ʱ��Xͨ������͵�ƽ��
//========================================================================
void PCA9685_Set_PWM(uint8_t Ch,uint16_t Value_On,uint16_t Value_Off)
{
	uint8_t Value_On_L;//�ߵ�ƽ�Ͱ�λ
	uint8_t Value_On_H;//�ߵ�ƽ��λ
  uint8_t Value_Off_L;//�͵�ƽ�Ͱ�λ
	uint8_t Value_Off_H;//�͵�ƽ��λ
	
	Value_On_H = (uint8_t)(Value_On>>8);//�ߵ�ƽ���ݲ��
	Value_On_L = (uint8_t)(Value_On&0x00ff);//�ߵ�ƽ���ݲ��
	Value_Off_H = (uint8_t)(Value_Off>>8);//�͵�ƽ���ݲ��
	Value_Off_L = (uint8_t)(Value_Off&0x00ff);//�͵�ƽ���ݲ��
	
	PCA9685_Write(0x06+Ch*4,Value_On_L);//����ON(�ߵ�ƽ)��ֵ�Ͱ�λ
	PCA9685_Write(0x06+Ch*4+1,Value_On_H);//����ON(�ߵ�ƽ)��ֵ�߰�λ
	PCA9685_Write(0x06+Ch*4+2,Value_Off_L);//����OFF(�͵�ƽ)��ֵ�Ͱ�λ
	PCA9685_Write(0x06+Ch*4+3,Value_Off_H);//����OFF(�͵�ƽ)��ֵ�߰�λ
}
//========================================================================
// ����: void PCA9685_Servo_Control(uint8_t Ch,uint8_t Angle)
// ����: ������ƺ���
// ����: Ch:ͨ��������Χ��0-15����Angle:����Ƕȣ���Χ��0-180��
//  	 
// ����: None.
// �汾: VER1.0.0
// ����: 2019-7-14
// ��ע: 
// 
//========================================================================
void PCA9685_Servo_Angle_Control(uint8_t Ch,uint8_t Angle)
{
	//0.5(102.4)-2.5(512)
	double Value;//�������ֵ�ø����ͱ���
	uint16_t PWM_Value;
	Value = Angle;
	Value = (Value/180*2+Servo_Range_Base)/20*4096;
	Value = floor(Value+0.5);
	PWM_Value = (uint16_t)Value;
	PCA9685_Set_PWM(Ch,0,PWM_Value);
}
//========================================================================
// ����: void PCA9685_Servo_Range_Control(uint8_t Ch,uint16_t Range)
// ����: ������ƺ���
// ����: Ch:ͨ��������Χ��0-15����Range:�����Χ����Χ��0-1000|45��-135�㣩
//  	 
// ����: None.
// �汾: VER1.0.0
// ����: 2019-7-14
// ��ע: 
// 
//========================================================================
void PCA9685_Servo_Range_Control(uint8_t Ch,uint16_t Range)
{
	//1.0(210)-2.0(410) //200
	double Value;//�������ֵ�ø����ͱ���
	uint16_t PWM_Value;
	Value = Range;
	Value = (Value+1050)/5;
	Value = floor(Value+0.5);
	PWM_Value = (uint16_t)Value;
	PCA9685_Set_PWM(Ch,0,PWM_Value);
}
//========================================================================
// ����: void Pca9685_ESC_Control(uint8_t Ch,double Range)
// ����: ������ƺ���
// ����: Ch:ͨ��������Χ��0-15����Range:������ţ���Χ��0-1000��
//  	 
// ����: None.
// �汾: VER1.0.0
// ����: 2019-7-14
// ��ע: 
// 
//========================================================================
void Pca9685_ESC_Control(uint8_t Ch,double Range)//���
{
	uint16_t ESC_Value;
	Range+=1065;
	Range/=5;
	Range = floor(Range+0.5);
	ESC_Value = (uint16_t)Range;
	PCA9685_Set_PWM(Ch,0,ESC_Value);
}
//========================================================================
// ����: uint8_t PCA9685_Init()
// ����: PCA9685��ʼ������
// ����: None.
//  	 
// ����: ��ʼ�������0���ɹ���1��ʧ�ܣ�.
// �汾: VER1.0.0
// ����: 2019-7-14
// ��ע: 
// 
//========================================================================
uint8_t PCA9685_Init()
{
	I2C0_Init();//����IIC0�˿����ʼ��IIC0
	PCA9685_Set_Freq(freq);//����Ƶ��Ϊ50HZ(20MS)
	if(Prescale == PCA9685_Read(0xFE))//���Ƶ�������Ƿ���ȷ
	{
		printf("PCA9685 Init [OK]!\r\n");
		return 0;
	}
	else
	{
		printf("PCA9685 Init [ERROR]!\r\n");
		//�˴�����ӱ������� or�ɿ�������������ֹ�����ջ���
		return 1;
	}
}
