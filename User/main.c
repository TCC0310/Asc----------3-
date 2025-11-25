#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Timer.h"
#include "Key.h"
#include "Motor.h"
#include "Encoder.h"
#include "Serial.h"
#include "Encoder.h"
#include "PWM.h"
#include "mycontrol.c"

uint8_t cnt=0,Key_Number;
float Mode1_Target,Mode1_Actual,Mode1_Out,Actual;
float Mode1_Kp=1,Mode1_Ki=0.3,Mode1_Kd=0.1;
float Mode1_Error0,Mode1_Error1,Mode1_Error2;

float Mode2_Target,Mode2_Actual,Mode2_Out;
float Mode2_Kp=1,Mode2_Ki=0.3,Mode2_Kd=0.1;
float Mode2_Error0,Mode2_Error1,Mode2_Error2;
int main(void)
{
	OLED_Init();
	Serial_Init();
  Timer_Init();
  Motor_Init();
  Encoder_Init();
	Key_Init();
	
	while (1)
	{

//     Key_Number=Key_GetNum();
//		 if(Key_Number==1)
//			{
//			  cnt++;
//			  if(cnt==2)cnt=0;
//			}
//			
//			
//			
////任务一：			
//			if(cnt==0)
//			{
//			   	Mode2_Out=0;
//			    Motor_SetPWM2(0);
//				if(Key_Number==2)
//			{
//			  Mode1_Target+=5;
//			  if(Mode1_Target>65)
//			{
//			  Mode1_Target=0;
//			} 
//			} 
//				OLED_Printf(0,0,OLED_8X16,"PWM:%+04.0f",Mode1_Target);
//				OLED_Printf(0,16,OLED_8X16,"Speed:%+04.0f",Mode1_Actual);
//				OLED_Printf(0,32,OLED_8X16,"OUT:%+04.0f",Mode1_Out);
//		    OLED_Update();
//			   if (Serial_GetRxFlag() == 1)
//		   {
//			   Mode1_Target = Serial_GetRxData();
//			   Serial_SendByte(Mode1_Target);
//		   }
//			 Serial_Printf("%f,%f,%f\r\n",Mode1_Target,Mode1_Actual,Mode1_Out);
//		  }
////任务二：			
//			if(cnt==1)
//			{
//				OLED_Clear();
//				OLED_Update();
//				Mode1_Error0=0,Mode1_Error1=0,Mode1_Error2=0;
//				Mode1_Out=0;
//			  Motor_SetPWM1(0);
//				Serial_Printf("%f,%f,%f\r\n",Mode2_Target,Mode2_Actual,Mode2_Out);
//			}
//	}
}


void TIM1_UP_IRQHandler(void)
{
	static uint16_t Count;
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
	{
		  TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    	Count++;
		   Key_tick();
			if(Count>=10)
			{
			//电机1控速
			if(cnt==0)
			{
			Count=0;
				
			Mode2_Error0=0;
			Mode2_Error1=0;
			Mode2_Error2=0;	
				
			//增量式PID控制速度
			Actual=Encoder_Get1();
			Mode1_Actual=Actual;
			Mode1_Error2=Mode1_Error1;
			Mode1_Error1=Mode1_Error0;
			Mode1_Error0=Mode1_Target-Mode1_Actual;
			
			Mode1_Out += Mode1_Kp * (Mode1_Error0-Mode1_Error1) +
		    	         Mode1_Ki *  Mode1_Error0 +
			             Mode1_Kd * (Mode1_Error0-2*Mode1_Error1+Mode1_Error2);
			
			Motor_SetPWM1(Mode1_Out);
		}		
			//电机2控速			
   	if(cnt==0)
			{
			Count=0;
				
			Mode2_Error0=0;
			Mode2_Error1=0;
			Mode2_Error2=0;	
				
			//增量式PID控制速度
			Actual=Encoder_Get1();
			Mode1_Actual=Actual;
			Mode1_Error2=Mode1_Error1;
			Mode1_Error1=Mode1_Error0;
			Mode1_Error0=Mode1_Target-Mode1_Actual;
			
			Mode1_Out += Mode1_Kp * (Mode1_Error0-Mode1_Error1) +
		    	         Mode1_Ki *  Mode1_Error0 +
			             Mode1_Kd * (Mode1_Error0-2*Mode1_Error1+Mode1_Error2);
			
			Motor_SetPWM1(Mode1_Out);
		}
//		//功能二主从电机
//		if (cnt==1)
//			{
//			Count=0;
//			
//			Mode1_Error0=0;
//			Mode1_Error1=0;
//			Mode1_Error2=0;	
//			
//			//读取电机一位置为目标
//			Mode2_Target+=Encoder_Get1();
//			//增量式PID控制位置
//			Mode2_Actual+=Encoder_Get2();
//			
//			Mode2_Error2=Mode2_Error1;
//			Mode2_Error1=Mode2_Error0;
//			Mode2_Error0=Mode2_Target-Mode2_Actual;
//			
//			Mode2_Out += Mode2_Kp * (Mode2_Error0-Mode2_Error1) +
//		    	   Mode2_Ki *  Mode2_Error0 +
//			       Mode2_Kd * (Mode2_Error0-2*Mode2_Error1+Mode2_Error2);
//			
//			if(Mode2_Out>100)Mode2_Out=100;
//			if(Mode2_Out<-100)Mode2_Out=-100;
//			
//			Motor_SetPWM2(Mode2_Out);
//		}		
	}
}
	
}







