#include "Control.h"
#include "PCA9685.h"
#include "math.h"
#include "SBUS.h"
#include "PID.h"
#include "MPU6050_UART.h"

extern SBUS_Buffer SBUS;
extern MPU6050_Data MPU6050_A;//加速度
extern MPU6050_Data MPU6050_W;//角速度
extern MPU6050_Data MPU6050_R;//角度

float Ch1;//副翼控制
float Ch2;//升降舵
float Ch3;//第三通道（油门）处理
float Ch4;//第四通道（方向舵）处理

float PID_Ch1;//副翼控制
float PID_Ch2;//升降舵

float x;
void ESC_Servo_Init()
{
	uint16_t i;
	//让三个舵机回中
	PCA9685_Servo_Range_Control(0,500);//副翼
	for(i=0;i<10000;i++);
	PCA9685_Servo_Range_Control(1,500);//升降翼
	for(i=0;i<10000;i++);
	PCA9685_Servo_Range_Control(3,500);//方向翼
	for(i=0;i<10000;i++);
	//以下效准电调行程
	PCA9685_Servo_Range_Control(2,1000);
	for(i=0;i<20000;i++);
	PCA9685_Servo_Range_Control(2,0);
	for(i=0;i<20000;i++);
	
	printf("ESC And Servo Init [OK]!\r\n");
}
void Accelerator_Control()
{
	Ch3=0.733*SBUS.Ch3-249.267;
	Ch3=floor(Ch3+0.5);
	if(Ch3>1000){Ch3=1000;}else if(Ch3<0){Ch3=0;}
	PCA9685_Servo_Range_Control(2,(uint16_t)Ch3);//三通道（油门）控制
}
void Direction_Control()
{
	if(SBUS.Ch4 == MID_Ch4_Value){Ch4 = 500;}
	else {Ch4 = -0.733*SBUS.Ch4 + 1249.267;Ch4=floor(Ch4+0.5);}
	if(Ch4>900){Ch4=900;}else if(Ch4<100){Ch4=100;}
		PCA9685_Servo_Range_Control(3,(uint16_t)Ch4);//方向舵控制
}
void Aileron_Control()
{
	if(SBUS.Ch1 == MID_Ch1_Value){Ch1 = 500;}
	else {Ch1 = -0.733*SBUS.Ch1 + 1249.267;Ch1=floor(Ch1+0.5);}
	if(Ch1>900){Ch1=900;}else if(Ch1<100){Ch1=100;}
		PCA9685_Servo_Range_Control(0,(uint16_t)Ch1);//副翼控制
}
void UP_DOWN_Control()
{
	if(SBUS.Ch2 == MID_Ch2_Value){Ch2 = 500;}
	/*else {Ch2 = 0.214*SBUS.Ch2 + 71.429;Ch2=floor(Ch2+0.5);}
	if(Ch2>800){Ch2=800;}else if(Ch2<200){Ch2=200;}*/
	else {Ch2 = 0.286*SBUS.Ch2 - 71.429;Ch2=floor(Ch2+0.5);}
	if(Ch2>900){Ch2=900;}else if(Ch2<100){Ch2=100;}
		PCA9685_Servo_Range_Control(1,(uint16_t)Ch2);//升降舵控制
}
void PID_Aileron_Control()
{
	if(SBUS.Ch1 == MID_Ch1_Value){PID_Ch1 = 0;}
	else {PID_Ch1 = -0.066*SBUS.Ch1 + 67.584;}
	if(PID_Ch1>45){PID_Ch1=45;}else if(PID_Ch1<-45){PID_Ch1=-45;}
	PCA9685_Servo_Range_Control(0,PID_Handle(Aileron,MPU6050_R.X,PID_Ch1));//副翼控制
}
void PID_UP_DOWN_Control()
{
	if(SBUS.Ch2 == MID_Ch2_Value){PID_Ch2 = 0;}
	else {PID_Ch2 = 0.032*SBUS.Ch2 - 64.286;}
	if(PID_Ch2>45){PID_Ch2=45;}else if(PID_Ch2<-45){PID_Ch2=-45;}
	x=PID_Ch2;
		PCA9685_Servo_Range_Control(1,PID_Handle(Lifting,MPU6050_R.Y,PID_Ch2));//升降舵控制
}
void Control_Handle(uint8_t Mode)
{
	if(Mode == Auto)
	{
    Accelerator_Control();
		Direction_Control();
		PID_Aileron_Control();
		PID_UP_DOWN_Control();
  }
	else if(Mode == Manual)
	{
		Accelerator_Control();
		Direction_Control();
		Aileron_Control();
		UP_DOWN_Control();
	}
	
}
uint8_t is_Auto_Manual(uint16_t Ch)
{
	if((Ch >= MIN_Auto_Channel_Value)&&(Ch <= MAX_Auto_Channel_Value))
	{
		return Auto;
	}
	
	else if((Ch >= MIN_Manual_Channel_Value)&&(Ch <= MAX_Manual_Channel_Value))
	{
		return Manual;
	}
}
