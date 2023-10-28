/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "DS3231.h"
#include "CLCD_I2C.h"
#include "stdio.h"
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
I2C_HandleTypeDef hi2c2;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

/* USER CODE BEGIN PV */
DS3231_Name DS3231;
CLCD_I2C_Name LCD1;
char str[80];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM2_Init(void);
static void MX_I2C2_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */
/*chuyen thanh kieu mang chua 2 ki tu de in ra lcd theo 1 format chung*/
typedef struct {
	int8_t Sec_Setting[2];
	int8_t Min_Setting[2];
	int8_t Hour_Setting[2];
}Time_Setting_TypeDef;

typedef struct {
	char Sec[10];
	char Min[10];
	char Hours[10];
}Clock_LCD_Show;

#define START_TIME_MAX 140
#define START_TIME_MIN 120
#define COMMAND_0  15

#define BUTTON_UP 0x18
#define BUTTON_DOWN 0x4A
#define BUTTON_OK 0x38
#define BUTTON_MODE 0x68

uint16_t Time1 = 0;
uint16_t Time2 = 0;
uint16_t Period = 0;

uint8_t counter_hex = 0; 
uint8_t command = 0;
uint32_t hex_ir = 0;

int8_t Mode_Setting = 0;

uint8_t Pos_Setting = 0;//position for setting


Time_Setting_TypeDef Time_Setting;
Clock_LCD_Show Clock_LCD;
uint32_t CurrentMillis = 0;
uint32_t PreviousMillis = 0;

uint8_t bool_lcd_show=1;

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void SetTime(void){
	DS3231_SetTime(&DS3231,Time_Setting.Hour_Setting[1]*10+Time_Setting.Hour_Setting[0],
			Time_Setting.Min_Setting[1]*10+Time_Setting.Min_Setting[0],Time_Setting.Sec_Setting[1]*10+Time_Setting.Sec_Setting[0]);
}

int8_t Time_limit(int8_t current_value,int8_t min, int8_t max){
	if (current_value > max) {
		current_value = min;
	}
	if (current_value < min) {
		current_value = max;
	}
	return current_value;
}

void TimeCount(int8_t count_1)
{
	switch(Pos_Setting){
		case 0:
			Time_Setting.Sec_Setting[0]+=count_1;
			Time_Setting.Sec_Setting[0] = Time_limit(Time_Setting.Sec_Setting[0],0,9);
			break;
		case 1:
			Time_Setting.Sec_Setting[1]+=count_1;
			Time_Setting.Sec_Setting[1] = Time_limit(Time_Setting.Sec_Setting[1],0,5);
			break;
		case 2:
			Time_Setting.Min_Setting[0]+=count_1;
			Time_Setting.Min_Setting[0] = Time_limit(Time_Setting.Min_Setting[0],0,9);
			break;
		case 3:
			Time_Setting.Min_Setting[1]+=count_1;
			Time_Setting.Min_Setting[1] = Time_limit(Time_Setting.Min_Setting[1],0,5);
			break;
		case 4:
			Time_Setting.Hour_Setting[0]+=count_1;
			Time_Setting.Hour_Setting[0] = Time_limit(Time_Setting.Hour_Setting[0],0,9);
			break;
		case 5:
			Time_Setting.Hour_Setting[1]+=count_1;
			if (Time_Setting.Hour_Setting[0]<=4)
				Time_Setting.Hour_Setting[1] = Time_limit(Time_Setting.Hour_Setting[1],0,2);
			else Time_Setting.Hour_Setting[1] = Time_limit(Time_Setting.Hour_Setting[1],0,1);
			break;
	}
}
void ControlOutput(void){
	switch(command){
				case BUTTON_MODE:	//button *
					Mode_Setting=!Mode_Setting;
					if (Mode_Setting) {
						Time_Setting.Sec_Setting[0] = DS3231.Sec%10;
						Time_Setting.Sec_Setting[1] = DS3231.Sec/10;
						Time_Setting.Min_Setting[0] = DS3231.Min%10;
						Time_Setting.Min_Setting[1] = DS3231.Min/10;
						Time_Setting.Hour_Setting[0] = DS3231.Hours%10;
						Time_Setting.Hour_Setting[1] = DS3231.Hours/10;
						Pos_Setting=0;
					}
					break;
				case BUTTON_UP:		
					if (Mode_Setting) {	
						TimeCount(1);
					}
					break;
				case BUTTON_DOWN:	
					if (Mode_Setting) {	
						TimeCount(-1);
					}
					break;	
				case BUTTON_OK:	
					if (Mode_Setting) {	
						Pos_Setting++;
						if (Pos_Setting >=6)
						{
							Pos_Setting = 0;
							Mode_Setting=!Mode_Setting;
							SetTime();
						}
					}
					break;
			}
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	Time2 = HAL_TIM_ReadCapturedValue(&htim2,TIM_CHANNEL_1);
	Period = Time2 - Time1;
	Time1 = Time2;
	
	if(Period < START_TIME_MAX && Period > START_TIME_MIN)
		{	
			hex_ir = 0;
			counter_hex = 0;
		}
		else if(Period > COMMAND_0) //bit 1
		{
			hex_ir |=(1<<(31-counter_hex));
			counter_hex++;
		}
		else if(Period < COMMAND_0) //bit 0
		{
			counter_hex++;
		}		
		
		if(counter_hex == 32) //ghi 32 lan
		{
			command = hex_ir>>8;
			ControlOutput();
		} 
}


void LCDConvertForm(void){
	
	if (DS3231.Sec<10) {
		sprintf(Clock_LCD.Sec,"0%d",DS3231.Sec);
	}
	else sprintf(Clock_LCD.Sec,"%d",DS3231.Sec);
	
	if (DS3231.Min<10) {
		sprintf(Clock_LCD.Min,"0%d",DS3231.Min);
	} 
	else sprintf(Clock_LCD.Min,"%d",DS3231.Min);
	
	if (DS3231.Hours<10) {
		sprintf(Clock_LCD.Hours,"0%d",DS3231.Hours);
	}
	else sprintf(Clock_LCD.Hours,"%d",DS3231.Hours);
}


void DisplayShow(){
	switch(Mode_Setting){
		case 0:
			if (bool_lcd_show==1) {
				CLCD_I2C_CursorOff(&LCD1);
				CLCD_I2C_BlinkOff(&LCD1);
				CLCD_I2C_Clear(&LCD1);
				CLCD_I2C_SetCursor(&LCD1,0,0);
				CLCD_I2C_WriteString(&LCD1,"TIME:");
				bool_lcd_show=0;
			}
			CurrentMillis = HAL_GetTick();
			if (CurrentMillis - PreviousMillis >=1000){	
				LCDConvertForm();
				DS3231_GetTime(&DS3231);
				CLCD_I2C_SetCursor(&LCD1,2,1);
				sprintf(str,"%s : %s : %s",Clock_LCD.Hours,Clock_LCD.Min,Clock_LCD.Sec);
				CLCD_I2C_WriteString(&LCD1,str);
				PreviousMillis = CurrentMillis;
			}
			
			break;
		case 1:
			if (bool_lcd_show==0) {
				CLCD_I2C_Clear(&LCD1);
				CLCD_I2C_SetCursor(&LCD1,0,0);
				CLCD_I2C_WriteString(&LCD1,"TIME SETTING:");
				CLCD_I2C_BlinkOn(&LCD1);
				bool_lcd_show=1;
			}
			CurrentMillis = HAL_GetTick();
			if (CurrentMillis - PreviousMillis >=1000){	
				CLCD_I2C_SetCursor(&LCD1,2,1);
				sprintf(str,"%d%d : %d%d : %d%d",Time_Setting.Hour_Setting[1],Time_Setting.Hour_Setting[0],
								Time_Setting.Min_Setting[1],Time_Setting.Min_Setting[0],Time_Setting.Sec_Setting[1],Time_Setting.Sec_Setting[0]);
				CLCD_I2C_WriteString(&LCD1,str);
				PreviousMillis = CurrentMillis;
			}
			switch(Pos_Setting){
				case 0:
					CLCD_I2C_SetCursor(&LCD1,13,1);
					break;
				case 1:
					CLCD_I2C_SetCursor(&LCD1,12,1);
					break;
				case 2:
					CLCD_I2C_SetCursor(&LCD1,8,1);
					break;
				case 3:
					CLCD_I2C_SetCursor(&LCD1,7,1);
					break;
				case 4:
					CLCD_I2C_SetCursor(&LCD1,3,1);
					break;
				case 5:
					CLCD_I2C_SetCursor(&LCD1,2,1);
					break;
			}
			break;
	}
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
  MX_TIM2_Init();
  MX_I2C2_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_1);
	DS3231_Init(&DS3231,&hi2c1);
	CLCD_I2C_Init(&LCD1,&hi2c2,0x7F,16,2);
	CLCD_I2C_Clear(&LCD1);
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		DisplayShow();
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
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
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

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
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 800-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 65535;
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
  if (HAL_TIM_IC_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 800-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 1000;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
