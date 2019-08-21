#ifndef __PID_H__
#define __PID_H__

#include "swm320.h"

#define MIN_Control_Angle 0.1 //最小控制角度

#define MidPoint 500//回中值

#define Aileron 0 //副翼
#define MAX_Aileron 900//副翼最大值
#define MIN_Aileron 100//副翼最小值

#define Lifting 1 //升降舵
#define MAX_Lifting 800//升降舵最大值
#define MIN_Lifting 200//升降舵最小值

#define MAX_Rudder 900//方向舵最大值
#define MIN_Rudder 100//方向舵最小值
typedef struct {
    float Kp_Aileron;			//比例系数
	  float Ti_Aileron;				//积分时间常数
    float Td_Aileron;				//微分时间常数
	
	  float Kp_Lifting;			//比例系数
	  float Ti_Lifting;				//积分时间常数
    float Td_Lifting;				//微分时间常数
	
    float Ek_Aileron;			//本次误差
    float Ek_1_Aileron;			//k-1次误差
	  float Ek_2_Aileron;			//k-2次误差
	
	  float Ek_Lifting;			//本次误差
    float Ek_1_Lifting;			//k-1次误差
	  float Ek_2_Lifting;			//k-2次误差
	

    float T;				//采样周期
    float Output_Lifting;		//升降舵PID输出
	  float Output_Aileron;		//副翼PID输出
} PID;

void PID_Init(void);
float PID_Handle(uint8_t Ch,float Feedback,float Point);
#endif


