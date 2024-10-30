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
#define LED1_Pin GPIO_PIN_0
#define LED1_GPIO_Port GPIOB
#define LED2_Pin GPIO_PIN_1
#define LED2_GPIO_Port GPIOB
#define KEY0_Pin GPIO_PIN_12
#define KEY0_GPIO_Port GPIOB
#define KEY0_EXTI_IRQn EXTI15_10_IRQn
#define KEY1_Pin GPIO_PIN_13
#define KEY1_GPIO_Port GPIOB
#define KEY1_EXTI_IRQn EXTI15_10_IRQn
#define KEY2_Pin GPIO_PIN_14
#define KEY2_GPIO_Port GPIOB
#define KEY2_EXTI_IRQn EXTI15_10_IRQn
#define KEY3_Pin GPIO_PIN_15
#define KEY3_GPIO_Port GPIOB
#define KEY3_EXTI_IRQn EXTI15_10_IRQn
#define BTN5_Pin GPIO_PIN_8
#define BTN5_GPIO_Port GPIOA
#define BTN6_Pin GPIO_PIN_9
#define BTN6_GPIO_Port GPIOA
#define BTN7_Pin GPIO_PIN_15
#define BTN7_GPIO_Port GPIOA
#define BTN8_Pin GPIO_PIN_3
#define BTN8_GPIO_Port GPIOB
#define BTN9_Pin GPIO_PIN_4
#define BTN9_GPIO_Port GPIOB
#define BTN10_Pin GPIO_PIN_5
#define BTN10_GPIO_Port GPIOB
#define BTN11_Pin GPIO_PIN_6
#define BTN11_GPIO_Port GPIOB
#define BTN12_Pin GPIO_PIN_7
#define BTN12_GPIO_Port GPIOB
#define BTN13_Pin GPIO_PIN_8
#define BTN13_GPIO_Port GPIOB
#define BTN14_Pin GPIO_PIN_9
#define BTN14_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */


#define MAX_CHANGE 50 // ��󵥴α仯��Χ
#define DEAD_ZONE 30  // ������Χ
#define STICK_ACTIVE_VAL 50 // ��ҡ�����ݱ仯�Ĵ�����������ֵ������Ϊҡ�˱�����
#define STICK_STATIC_VAL -50 // ��ҡ�����ݲ��仯�Ĵ�����������ֵ������Ϊҡ�˾�ֹ

#define THROTTLE_ENABLED				1					// 1 for enabled, 0 for disabled
#define RUDDER_ENABLED					1					// 1 for enabled, 0 for disabled
#define JOYSTICK_AXIS_NUM				3
#define SECONDARY_AXIS_NUM			3

#define BUTTONS_NUM							14
#define LEDS_NUM								0

/* END OF USER CODE AREA */



/* DO NOT CHANGE CODE BELOW IF NOT SURE WHAT ARE YOU DOING */

#define FILTER_WINDOW_SIZE			10

#define ADC_CHANNELS_NUM				8


#define AXIS_NUM								((THROTTLE_ENABLED) + (JOYSTICK_AXIS_NUM) \
																+ (SECONDARY_AXIS_NUM) + (RUDDER_ENABLED))
																
#if (JOYSTICK_AXIS_NUM > 0)
	#define JOYSTICK_AXIS_ENABLED  	1
#else
	#define JOYSTICK_AXIS_ENABLED  	0
#endif

#if (SECONDARY_AXIS_NUM > 0)
	#define SECONDARY_AXIS_ENABLED  	1
#else
	#define SECONDARY_AXIS_ENABLED  	0
#endif

#if (BUTTONS_NUM > 0)
	#define BUTTONS_ENABLED  	1
#else
	#define BUTTONS_ENABLED  	0
#endif

#define DEVICE_ID1					0x1FFFF7E8
#define DEVICE_ID2					0x1FFFF7EA
#define DEVICE_ID3					0x1FFFF7EC
#define DEVICE_ID4					0x1FFFF7F0


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
