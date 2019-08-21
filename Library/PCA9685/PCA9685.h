/**
  ******************************************************************************
  * @file    PCA9685.H
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
#ifndef _PCA9685_H_
#define _PCA9685_H_

#include "swm320.h"

#define PCA9685_ADDR 0x40 //0x40+A5:A0 7λIIC��ַ
#define FOSC_MAIN 25000000UL //ʹ���ڲ�25MHz����
#define contant 0.915 //���У׼
#define freq 50 //Ƶ��

#define Range_MAX 413 //���������2ms��2.016ms����ռ�ձȣ�
#define Range_MIN 213 //���������1ms (1.040ms)��ռ�ձȣ�

#define Servo_Range_Base 0.5 //���

#define delay_contant 3500 //����PCA9685�����ֲᣬÿ��IIC��������Ҫ�����ӳ�

uint8_t PCA9685_Init(void);//PCA9685��ʼ������
void Pca9685_ESC_Control(uint8_t Ch,double Range);//������ƺ���
void PCA9685_Servo_Angle_Control(uint8_t Ch,uint8_t Angle);//����Ƕȿ��ƺ���
void PCA9685_Servo_Range_Control(uint8_t Ch,uint16_t Range);//������ſ��ƺ���
#endif

