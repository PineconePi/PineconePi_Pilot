#include "PID.h"

uint8_t Pid_Freq;
float Value_Lifting=0;
float Value_Aileron=0;
PID Pid;
void PID_Init()
{
	TIMR_Init(TIMR0, TIMR_MODE_TIMER, 4800000, 1);	//每0.04秒钟触发一次中断
	TIMR_Start(TIMR0);
//	
	Pid.Kp_Aileron = 11.1;
	Pid.Td_Aileron = 0.0008;//0.0028;
	Pid.Ti_Aileron = 1;//0.009;
//	
//	Pid.Kp_Lifting = 11.1;
//	Pid.Td_Lifting = 0.0008;//0.0028;
//	Pid.Ti_Lifting = 1;//0.009;
	
//	Pid.Kp_Aileron = 22.2;
//	Pid.Td_Aileron = 0.00005;//0.0028;
//	Pid.Ti_Aileron = 5.25;//0.009;
//	
	Pid.Kp_Lifting = 22.2;
	Pid.Td_Lifting = 0.00005;//0.0028;
	Pid.Ti_Lifting = 5.25;//0.009;
	
//	Pid.Kp_Lifting = 11.1;
//	Pid.Td_Lifting = 0.00001;//0.0028;
//	Pid.Ti_Lifting = 3.25;//0.009;
	
	Pid.T = 0.04;
	
	printf("PID Init [OK]!\r\n");
}
void TIMR0_Handler(void)
{
	TIMR_INTClr(TIMR0);
	Pid_Freq=1;
}
float PID_Handle(uint8_t Ch,float Feedback,float Point)
{
	if(Pid_Freq==1)
	{
		if(Ch==Aileron)
		{
		if(1)
		{
		Pid_Freq = 0;
		Pid.Ek_Aileron = Point - Feedback;
		if((Pid.Ek_Aileron<MIN_Control_Angle)&&(Pid.Ek_Aileron>(-MIN_Control_Angle)))
		{
			Pid.Ek_Aileron=0;
		}
		Pid.Output_Aileron = Pid.Kp_Aileron*( Pid.Ek_Aileron -  Pid.Ek_1_Aileron)
		           + (( Pid.Kp_Aileron* Pid.T)/ Pid.Ti_Aileron)* Pid.Ek_Aileron
		           + ((( Pid.Kp_Aileron* Pid.Td_Aileron)/ Pid.T)*( Pid.Ek_Aileron- 2*Pid.Ek_1_Aileron- Pid.Ek_2_Aileron));
		Pid.Ek_1_Aileron=Pid.Ek_Aileron;
		Pid.Ek_2_Aileron=Pid.Ek_1_Aileron;
		Value_Aileron+=Pid.Output_Aileron;
		if(Value_Aileron<MIN_Aileron){Value_Aileron =MIN_Aileron;}else if(Value_Aileron>MAX_Aileron){Value_Aileron = MAX_Aileron;}//else if(Point==0){Value_Aileron=MidPoint;}
		return Value_Aileron;
	}
/*		else if(Feedback < 0)
		{
		Pid_Freq = 0;
		Pid.Ek_Aileron = Point - Feedback;
		if((Pid.Ek_Aileron<MIN_Control_Angle)&&(Pid.Ek_Aileron>(-MIN_Control_Angle)))
		{
			Pid.Ek_Aileron=0;
		}
		Pid.Output_Aileron= Pid.Kp_Aileron*( Pid.Ek_Aileron -  Pid.Ek_1_Aileron)
		           + (( Pid.Kp_Aileron* Pid.T)/ Pid.Ti_Aileron)* Pid.Ek_Aileron
		           + ((( Pid.Kp_Aileron* Pid.Td_Aileron)/ Pid.T)*( Pid.Ek_Aileron- 2*Pid.Ek_1_Aileron- Pid.Ek_2_Aileron));
		Pid.Ek_1_Aileron=Pid.Ek_Aileron;
		Pid.Ek_2_Aileron=Pid.Ek_1_Aileron;
		Value_Aileron+=(Pid.Output_Aileron);
		if(Value_Aileron<MidPoint){Value_Aileron = MidPoint;}elseif(Value_Aileron<MIN_Aileron){Value_Aileron =MIN_Aileron;} else if(Value_Aileron>MAX_Aileron){Value_Aileron = MAX_Aileron;}
		return Value_Aileron;
		}*/
	}
		if(Ch == Lifting)
		{
		if(1)
		{
		Pid_Freq = 0;
		Pid.Ek_Lifting = Point - Feedback;
		if((Pid.Ek_Lifting<MIN_Control_Angle)&&(Pid.Ek_Lifting>(-MIN_Control_Angle)))
		{
			Pid.Ek_Lifting=0;
		}
		Pid.Output_Lifting = Pid.Kp_Lifting*( Pid.Ek_Lifting -  Pid.Ek_1_Lifting)
		           + (( Pid.Kp_Lifting* Pid.T)/ Pid.Ti_Lifting)* Pid.Ek_Lifting
		           + ((( Pid.Kp_Lifting* Pid.Td_Lifting)/ Pid.T)*( Pid.Ek_Lifting- 2*Pid.Ek_1_Lifting- Pid.Ek_2_Lifting));
		Pid.Ek_1_Lifting=Pid.Ek_Lifting;
		Pid.Ek_2_Lifting=Pid.Ek_1_Lifting;
		Value_Lifting+=Pid.Output_Lifting;//
		if(Value_Lifting<MIN_Lifting){Value_Lifting =MIN_Lifting;}else if(Value_Lifting>MAX_Lifting){Value_Lifting = MAX_Lifting;}//else if(Point==0){Value_Lifting=MidPoint;}
		return Value_Lifting;
	}
	/*	else if(Feedback < 0)
		{
		Pid_Freq = 0;
		Pid.Ek_Lifting = Point - Feedback;
		if((Pid.Ek_Lifting<MIN_Control_Angle)&&(Pid.Ek_Lifting>(-MIN_Control_Angle)))
		{
			Pid.Ek_Lifting=0;
		}
		Pid.Output_Lifting= Pid.Kp_Lifting*( Pid.Ek_Lifting -  Pid.Ek_1_Lifting)
		           + (( Pid.Kp_Lifting* Pid.T)/ Pid.Ti_Lifting)* Pid.Ek_Lifting
		           + ((( Pid.Kp_Lifting* Pid.Td_Lifting)/ Pid.T)*( Pid.Ek_Lifting- 2*Pid.Ek_1_Lifting- Pid.Ek_2_Lifting));
		Pid.Ek_1_Lifting=Pid.Ek_Lifting;
		Pid.Ek_2_Lifting=Pid.Ek_1_Lifting;
		Value_Lifting+=Pid.Output_Lifting;
		if(Value_Lifting<=MidPoint){Value_Lifting = MidPoint;}else if(Value_Lifting>=MAX_Lifting){Value_Lifting = MAX_Lifting;}
		return Value_Lifting;
		}*/
		}	
}
	else
	{
		if(Ch == Lifting){return Value_Lifting;}
		if(Ch==Aileron){return Value_Aileron;}
	}
}
