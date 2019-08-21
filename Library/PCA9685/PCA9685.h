/**
  ******************************************************************************
  * @file    PCA9685.H
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
#ifndef _PCA9685_H_
#define _PCA9685_H_

#include "swm320.h"

#define PCA9685_ADDR 0x40 //0x40+A5:A0 7位IIC地址
#define FOSC_MAIN 25000000UL //使用内部25MHz晶振
#define contant 0.915 //误差校准
#define freq 50 //频率

#define Range_MAX 413 //电调满油门2ms（2.016ms）（占空比）
#define Range_MIN 213 //电调无油门1ms (1.040ms)（占空比）

#define Servo_Range_Base 0.5 //舵机

#define delay_contant 3500 //根据PCA9685数据手册，每次IIC结束后，需要进行延迟

uint8_t PCA9685_Init(void);//PCA9685初始化函数
void Pca9685_ESC_Control(uint8_t Ch,double Range);//电调控制函数
void PCA9685_Servo_Angle_Control(uint8_t Ch,uint8_t Angle);//舵机角度控制函数
void PCA9685_Servo_Range_Control(uint8_t Ch,uint16_t Range);//舵机油门控制函数
#endif

