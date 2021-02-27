/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */
char text[1] = "";
char seconds_s[8] = "0";
char* status="";
int minutes=0,seconds=0, flag_led=0,flag_set=0,flag_clr=0,flag_time=0;
int array[6];
int i=0,dv;
int j=11;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int keyboard(int delay);     // 
void clear()
{
	if(flag_clr==1)
	{
		lcd_clear_display();
		flag_clr=0;
	}
	
}
void clear_time()
{
	if(flag_time==1)
	{
		seconds=0;
		minutes=0;
		dv=0;
		for(int i=0;i<7;i++)
		{
			array[i]=0;
		}
		flag_time=0;
	}
	
}
int convert()
{
	 j-=2;
	 for(int k=11;k<=j;k++)
	 {
			dv+=array[j-k]*pow(10,(k-11));
	 }
	 return dv;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_Delay(1000);
	
	lcd_init();
	j=11;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		//-----------------------------------HANG 1
		clear();                              // clear lcd
		lcd_goto_XY(1,0);                     
		lcd_send_string("so giay: ");
		HAL_Delay(5);
		if(seconds==0)												// test seconds
		{
			lcd_goto_XY(2,0);
			lcd_send_string("led: OFF ");
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,0);
		}
		else if(seconds!=0)
		{
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,1);
			lcd_goto_XY(2,0);
			lcd_send_string("led: ON");           
		}
			sprintf(seconds_s,"%d",seconds);
			strcat(seconds_s,"        ");
			lcd_goto_XY(1,10);
			lcd_send_string(seconds_s);					//print sencond to lcd
		 if(keyboard(10)==1)									// time for get key is 10ms
		 {
			 if(*text == '#' )
			 {
				 lcd_clear_display();
				 
				 flag_set=1;
				 flag_clr=1;
				 flag_time=1;
				 HAL_Delay(3000);
			 }
		 }
		 
		 while(flag_set==1)
		{
			clear();
			clear_time();
			lcd_goto_XY(1,0);
			lcd_send_string("nhap phut: ");
			lcd_goto_XY(2,0);
			lcd_send_string("led: OFF ");
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,0);
			if(keyboard(20)==1)
		 {
			lcd_goto_XY(1,j);
			array[j-11]=(int)*text-48;
			lcd_send_string(text);
			 j+=1;
			 if(*text == '*' )		//set mode import time
			 {
				 flag_clr=1;				//set flag clear lcd 
				 flag_set=2;				//flag mode
				 minutes=convert();
				 dv=0;
				 j=11;
				 lcd_clear_display();
				 HAL_Delay(1000);
			 }
			 else if(*text == '#' )		//set mode out
			 {
				 flag_set=0;				
				 flag_clr=1;
//				 j-=2;
//				 minutes=convert();
//				 seconds=minutes*60;
				 dv=0;
				 j=11;
			 }
		 }
		}
		while(flag_set==2)
		{
			clear();
			clear_time();
			lcd_goto_XY(1,0);
			lcd_send_string("nhap giay: ");
			lcd_goto_XY(2,0);
			lcd_send_string("led: OFF ");
				
			if(keyboard(40)==1)
		 {
			lcd_goto_XY(1,j);
			array[j-11]=(int)*text-48;
			 
			lcd_send_string(text);
			 j+=1;
			 if(*text == '*' )
			 {
				 flag_set=0;
				 flag_clr=1;
				 flag_time=1;
				 seconds= convert();
				 seconds+=minutes*60;
				 j=11;
			 }
			 if(*text == '#' )
			 {
				 flag_set=0;
				 flag_clr=1;
				 j-=2;
				 seconds=convert();
				 seconds+=minutes*60;
				 j=11;
			 }
			 else if(*text == 'C' )
			 {
				 lcd_clear_display();
				 flag_set=0;
				 flag_led=0;
			 }
		 }
		}
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 36000;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 1999;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 36000;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA4 PA5 PA6 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
	{
		if(seconds!=0)
			{
				if(htim->Instance==htim2.Instance)     //timer 2 count down
				{
//					HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
					seconds-=1;
				}
			}
	}

int keyboard(int delay)
{
		 i=0;
		text[0]=' ';
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,1);
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)==1)
		{
			while(i<delay)
			{
				i+=1;
				if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)!=1)
					return 0;
				HAL_Delay(1);
			}
			text[0]='1';
			while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)==1);
			return 1;
		}
		else if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)==1)
		{
			while(i<delay)
			{
				i+=1;
				if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)!=1)
					return 0;
				HAL_Delay(1);
			}
			text[0]='2';
			while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)==1);
			return 1;
		}
		else if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==1)
		{
			while(i<delay)
			{
				i+=1;
				if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)!=1)
					return 0;
				HAL_Delay(1);
			}
			text[0]='3';
			while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==1);
			return 1;
		}
		else if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)==1)
		{
			while(i<delay)
			{
				i+=1;
				if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)!=1)
					return 0;
				HAL_Delay(1);
			}
			text[0]='A';
			while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)==1);
			return 1;
		}
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,0);
//		HAL_Delay(100);
		//-----------------------------------HANG 2
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,1);
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)==1)
		{
			i=0;
			while(i<delay)
			{
				i+=1;
				if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)!=1)
					return 0;
				HAL_Delay(1);
			}
			text[0]='4';
			while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)==1);
			return 1;
		}
		else if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)==1)
		{
			while(i<delay)
			{
				i+=1;
				if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)!=1)
					return 0;
				HAL_Delay(1);
			}
			text[0]='5';
			while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)==1);
			return 1;
		}
		else if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==1)
		{
			while(i<delay)
			{
				i+=1;
				if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)!=1)
					return 0;
				HAL_Delay(1);
			}
			text[0]='6';
			while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==1);
			return 1;
		}
		else if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)==1)
		{
			while(i<delay)
			{
				i+=1;
				if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)!=1)
					return 0;
				HAL_Delay(1);
			}
			text[0]='B';
			while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)==1);
			return 1;
		}
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,0);
		HAL_Delay(100);
		//-----------------------------------HANG 3
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,1);
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)==1)
		{
			while(i<delay)
			{
				i+=1;
				if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)!=1)
					return 0;
				HAL_Delay(1);
			}
			text[0]='7';
			while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)==1);
			return 1;
		}
		else if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)==1)
		{
			while(i<delay)
			{
				i+=1;
				if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)!=1)
					return 0;
				HAL_Delay(1);
			}
			text[0]='8';
			while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)==1);
			return 1;
		}
		else if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==1)
		{
			while(i<delay)
			{
				i+=1;
				if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)!=1)
					return 0;
				HAL_Delay(1);
			}
			text[0]='9';
			while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==1);
			return 1;
		}
		else if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)==1)
		{
			while(i<delay)
			{
				i+=1;
				if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)!=1)
					return 0;
				HAL_Delay(1);
			}
			text[0]='C';
			while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)==1);
			return 1;
		}
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,0);
		HAL_Delay(100);
		//-----------------------------------HANG 4
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,1);
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)==1)
		{
			while(i<delay)
			{
				i+=1;
				if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)!=1)
					return 0;
				HAL_Delay(1);
			}
			text[0]='*';
			while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)==1);
			return 1;
		}
		else if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)==1)
		{
			while(i<delay)
			{
				i+=1;
				if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)!=1)
					return 0;
				HAL_Delay(1);
			}
			text[0]='0';
			while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)==1);
			return 1;
		}
		else if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==1)
		{
			while(i<delay)
			{
				i+=1;
				if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)!=1)
					return 0;
				HAL_Delay(1);
			}
			text[0]='#';
			while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==1);
			return 1;
		}
		else if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)==1)
		{
			while(i<delay)
			{
				i+=1;
				if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)!=1)
					return 0;
				HAL_Delay(1);
			}
			text[0]='D';
			while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)==1);
			return 1;
		}
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,0);
		HAL_Delay(100);
}
	
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
