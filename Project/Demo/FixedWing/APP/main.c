/**
  ******************************************************************************
  * @Project    FixedWing
  * @author  PineconePi(xdd_core)
  * @version V1.0.0
  * @date    02-08-2019
  * @brief  
  * @License:GNU General Public License v3.0         
  ******************************************************************************
  * @attention
  *
  *
  *  
  * 
  * 
  * 
  * 
  *
  * 
  ******************************************************************************
	**/
#include "SWM320.h"
#include "MPU6050_UART.h"
#include "SBUS.h"
#include "PID.h"
#include "PCA9685.h"
#include "Log.h"
#include "Control.h"

extern SBUS_Buffer SBUS;

int main(void)
{ 
	SystemInit();
  Log_Init();
	SBUS_Init();
  MPU6050_Init();
	PID_Init();
	PCA9685_Init();
	ESC_Servo_Init();
	while(1)
	{
		SBUS_Handle();
		MPU6050_UART_Handle();
		Control_Handle(is_Auto_Manual(SBUS.Ch7));
	}
}
