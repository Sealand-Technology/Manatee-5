/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#define BTN0_Pin GPIO_PIN_12
#define BTN0_GPIO_Port GPIOB
#define BTN1_Pin GPIO_PIN_13
#define BTN1_GPIO_Port GPIOB
#define BTN2_Pin GPIO_PIN_14
#define BTN2_GPIO_Port GPIOB
#define BTN3_Pin GPIO_PIN_15
#define BTN3_GPIO_Port GPIOB
#define BTN4_Pin GPIO_PIN_8
#define BTN4_GPIO_Port GPIOA
#define BTN5_Pin GPIO_PIN_9
#define BTN5_GPIO_Port GPIOA
#define BTN6_Pin GPIO_PIN_15
#define BTN6_GPIO_Port GPIOA
#define BTN7_Pin GPIO_PIN_3
#define BTN7_GPIO_Port GPIOB
#define BTN8_Pin GPIO_PIN_4
#define BTN8_GPIO_Port GPIOB
#define BTN9_Pin GPIO_PIN_5
#define BTN9_GPIO_Port GPIOB
#define BTN10_Pin GPIO_PIN_6
#define BTN10_GPIO_Port GPIOB
#define BTN11_Pin GPIO_PIN_7
#define BTN11_GPIO_Port GPIOB
#define BTN12_Pin GPIO_PIN_8
#define BTN12_GPIO_Port GPIOB
#define BTN13_Pin GPIO_PIN_9
#define BTN13_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

#define MAX_CHANGE             50
#define DEAD_ZONE              30
#define STICK_ACTIVE_VAL       50
#define STICK_STATIC_VAL      -50

#define ADC_CHANNEL_NUM				  8
#define AXIS_NUM							  8
#define BUTTON_NUM						 14		

#if (BUTTON_NUM > 0)
	#define BUTTONS_ENABLED  	1
#else
	#define BUTTONS_ENABLED  	0
#endif

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
