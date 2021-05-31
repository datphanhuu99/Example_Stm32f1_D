/*
 * MOTOR.h
 *
 *  Created on: Nov 13, 2020
 *      Author: quanp
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private defines -----------------------------------------------------------*/
#define	Back		0
#define Forward	1

#define Length  					16.5
#define Width   					19
#define Max_Angle  				55
#define PI      					3.14159
#define WheelSize					18.8

#define AngleServoRatio		38/50
#define Left_Ratio				35/50
#define Right_Ratio				1
#define SpeedLeftRightRatio	0.67

#define	_Speed						100
#define Duty_Per_Round		340


/* Exported functions prototypes ---------------------------------------------*/
void LeftMotor(int dir, int speed);
void RightMotor(int dir, int speed);


void RunRight90 (int anggle, int speed);
void RunLeft90  (int anggle, int speed);
void setAngle(int8_t angle);
int32_t Encoder1_Get_Counter(void);
int32_t Encoder2_Get_Counter(void);

void Run(int8_t angle, uint8_t speed);
//add by dat
//int32_t pid_Left(double speed_L, int8_t duty_L, float kp_L, float ki_L, float kd_L);
void Set_Encoder(void);

//end
#endif /* INC_MOTOR_H_ */
