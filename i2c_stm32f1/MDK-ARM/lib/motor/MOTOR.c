/*
 * MOTOR.c
 *
 *  Created on: Nov 13, 2020
 *      Author: quanp
 */

/* Includes ------------------------------------------------------------------*/
#include "MOTOR.h"
#include "main.h"
#include <math.h>
#include <stdio.h>
#include "GPIO.h"
#include "TIMER.h"
/* Private variables ---------------------------------------------------------*/
__IO int32_t Num_Encoder_1 = 0;
__IO int32_t Num_Encoder_2 = -70000;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim1;
// begin main.c
//#define kp_L 0     
//#define ki_L 0
//#define kd_L 0

//#define kp_R 0
//#define ki_R 0
//#define kd_R 0     
//end main.c
//double i,d,preError,precount_L;
void LeftMotor(int dir, int speed){
	HAL_GPIO_WritePin(MotorLeft_Dir_GPIO_Port, MotorLeft_Dir_pin, (GPIO_PinState) dir);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,speed);
}
void RightMotor(int dir, int speed){
	HAL_GPIO_WritePin(MotorRight_Dir_GPIO_Port, MotorRight_Dir_pin, (GPIO_PinState) dir);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, speed);
}

void RunLeft90 (int anggle, int speed){
	//count down
	
	uint32_t Num_Encoder_pre = 0, Num_Encoder_next = 0;
	
	double a = tan (anggle * AngleServoRatio* PI / 180);	
	float SpeedRatioCalculator = (Length / a)/((Length / a) + Width);
	float GetLargerDistance = ((Length / a) + Width)*PI/2;
	float GetSmallerDistance = (Length / a)*PI/2;
	float GetRound = GetLargerDistance/WheelSize;
	float GetFreq = GetRound*Duty_Per_Round;
	
	Num_Encoder_pre = Encoder2_Get_Counter();
	while (1){
		setAngle(-anggle*Left_Ratio);
		RightMotor(Forward, speed);
		LeftMotor(Forward,speed*SpeedRatioCalculator*SpeedLeftRightRatio);
		Num_Encoder_next = Encoder2_Get_Counter();
		
		//*****************
//		HAL_Delay(2000);
//			break;
		if ((-Num_Encoder_next + Num_Encoder_pre) > GetFreq){
			break;
		}
	}
	
}

void RunRight90 (int anggle, int speed){
	// count up
	
	uint32_t Num_Encoder_pre = 0, Num_Encoder_next = 0;
	double a = tan (anggle * AngleServoRatio* PI / 180);	
	float SpeedRatioCalculator = (Length / a)/((Length / a) + Width);
	float GetLargerDistance = ((Length / a) + Width)*PI/2;
//	float GetSmallerDistance = (Length / a)*PI/2;
	float GetRound = GetLargerDistance/WheelSize ;
	float GetFreq = GetRound*Duty_Per_Round;	
	
	Num_Encoder_pre = Encoder1_Get_Counter();
	while (1){
		setAngle(anggle);
		RightMotor(Forward, speed);
		LeftMotor(Forward, speed*SpeedRatioCalculator);
		Num_Encoder_next = Encoder1_Get_Counter();

		if ((Num_Encoder_next - Num_Encoder_pre) > GetFreq){
			break;
		}
	}
	
}

int32_t Encoder1_Get_Counter(void)
{
	return TIM2->CNT ;
}
int32_t Encoder2_Get_Counter(void)
{
	return TIM3->CNT ;
}

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//	if(htim->Instance == TIM2)
//	{
//		if(TIM2->CR1 & 0x10)
//		{
//			Num_Encoder_1 -= 0xFFFF;
//		}
//		else
//		{
//			Num_Encoder_1 += 0xFFFF;
//		}
//	}
//	if(htim->Instance == TIM3)
//	{
//		if(TIM3->CR1 & 0x10)
//		{
//			Num_Encoder_2 -= 0xFFFF;
//		}
//		else
//		{
//			Num_Encoder_2 += 0xFFFF;
//		}
//	}
//	if(htim->Instance==htim4.Instance)
//	{
//		count_tim++;
//		if(count_tim>=5)
//		{
//			flag=1;
//		}
//	}
////	if(htim->Instance == TIM4)
////	{
////		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
////		
////	}
//}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM4)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
	}
}

void setAngle(int8_t angle)
{
	int32_t Servo_Pulse = 0;
	if(angle<Neg_Servo_Angle)
	{
		angle = Neg_Servo_Angle;
	}
	if(angle > Pos_Servo_Angle)
	{
		angle = Pos_Servo_Angle;
	}
	Servo_Pulse = Timer_Servo_Center + angle * Timer_Servo_Step_Angle;
	TIM4->CCR1 = Servo_Pulse;
//	setPWM(&htim4, TIM_CHANNEL_1, Timer_Servo_Period, Servo_Pulse);
}
void Set_Encoder(void)
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      {
	TIM2->CNT=32767;
	TIM3->CNT=32767;
}
void Run(int8_t angle, uint8_t speed){
	double a = tan (angle * AngleServoRatio* PI / 180);	
	float SpeedRatioCalculator = (Length / a)/((Length / a) + Width);
	if(angle < 0){
		setAngle(angle*Left_Ratio);
		RightMotor(Forward, speed);
		LeftMotor(Forward, speed*SpeedRatioCalculator);
	}
	if (angle > 0){
		setAngle(angle);
		LeftMotor(Forward, speed);
		RightMotor(Forward, speed*SpeedRatioCalculator);
	}
}

//int32_t pid_Left(double speed_L, int8_t duty_L, float kp_L, float ki_L, float kd_L)
//{
////		speed_L=speed_L/60;
//		int count_value = Encoder1_Get_Counter()-32767;
//		double error = (speed_L)*5-count_value/10;
////		duty_L=duty_L+error*0.001;
//		i=i+error;
//		d=(error-preError);
//		duty_L=duty_L+(kp_L*error)+(ki_L*i)+(kd_L*d);
//		preError=error;
//		//luu data
////		data_s[ds]=ds;
////		data_duty[ds]=duty;
////		d_motor[ds]=data_motor;
////		d_value[ds]=value;
////		data_count[ds]=count_value;
//		precount_L=count_value;
//	
//	return duty_L;
//}
