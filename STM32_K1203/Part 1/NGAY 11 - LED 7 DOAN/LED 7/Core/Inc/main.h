/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SEGA_Pin GPIO_PIN_0
#define SEGA_GPIO_Port GPIOA
#define SEGB_Pin GPIO_PIN_1
#define SEGB_GPIO_Port GPIOA
#define SEGF_Pin GPIO_PIN_2
#define SEGF_GPIO_Port GPIOA
#define SEGG_Pin GPIO_PIN_3
#define SEGG_GPIO_Port GPIOA
#define SEGC_Pin GPIO_PIN_4
#define SEGC_GPIO_Port GPIOA
#define SEGD_Pin GPIO_PIN_5
#define SEGD_GPIO_Port GPIOA
#define SEGE_Pin GPIO_PIN_6
#define SEGE_GPIO_Port GPIOA
#define SEGH_Pin GPIO_PIN_7
#define SEGH_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
