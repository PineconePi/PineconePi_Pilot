#ifndef __PID_H__
#define __PID_H__

#include "swm320.h"

#define MIN_Control_Angle 0.1 //��С���ƽǶ�

#define MidPoint 500//����ֵ

#define Aileron 0 //����
#define MAX_Aileron 900//�������ֵ
#define MIN_Aileron 100//������Сֵ

#define Lifting 1 //������
#define MAX_Lifting 800//���������ֵ
#define MIN_Lifting 200//��������Сֵ

#define MAX_Rudder 900//��������ֵ
#define MIN_Rudder 100//�������Сֵ
typedef struct {
    float Kp_Aileron;			//����ϵ��
	  float Ti_Aileron;				//����ʱ�䳣��
    float Td_Aileron;				//΢��ʱ�䳣��
	
	  float Kp_Lifting;			//����ϵ��
	  float Ti_Lifting;				//����ʱ�䳣��
    float Td_Lifting;				//΢��ʱ�䳣��
	
    float Ek_Aileron;			//�������
    float Ek_1_Aileron;			//k-1�����
	  float Ek_2_Aileron;			//k-2�����
	
	  float Ek_Lifting;			//�������
    float Ek_1_Lifting;			//k-1�����
	  float Ek_2_Lifting;			//k-2�����
	

    float T;				//��������
    float Output_Lifting;		//������PID���
	  float Output_Aileron;		//����PID���
} PID;

void PID_Init(void);
float PID_Handle(uint8_t Ch,float Feedback,float Point);
#endif


