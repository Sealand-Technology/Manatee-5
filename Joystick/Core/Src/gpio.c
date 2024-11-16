/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pins : PBPin PBPin PBPin PBPin
                           PBPin PBPin PBPin PBPin
                           PBPin PBPin PBPin */
  GPIO_InitStruct.Pin = BTN0_Pin|BTN1_Pin|BTN2_Pin|BTN3_Pin
                          |BTN7_Pin|BTN8_Pin|BTN9_Pin|BTN10_Pin
                          |BTN11_Pin|BTN12_Pin|BTN13_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PAPin PAPin PAPin */
  GPIO_InitStruct.Pin = BTN4_Pin|BTN5_Pin|BTN6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

void GPIO_Poll(uint16_t *buttons_state)
{
  *buttons_state = 0;
  *buttons_state |= HAL_GPIO_ReadPin(BTN0_GPIO_Port, BTN0_Pin);
  *buttons_state |= HAL_GPIO_ReadPin(BTN1_GPIO_Port, BTN1_Pin) << 1;
  *buttons_state |= HAL_GPIO_ReadPin(BTN2_GPIO_Port, BTN2_Pin) << 2;
  *buttons_state |= HAL_GPIO_ReadPin(BTN3_GPIO_Port, BTN3_Pin) << 3;
  *buttons_state |= HAL_GPIO_ReadPin(BTN4_GPIO_Port, BTN4_Pin) << 4;
  *buttons_state |= HAL_GPIO_ReadPin(BTN5_GPIO_Port, BTN5_Pin) << 5;
  *buttons_state |= HAL_GPIO_ReadPin(BTN6_GPIO_Port, BTN6_Pin) << 6;
  *buttons_state |= HAL_GPIO_ReadPin(BTN7_GPIO_Port, BTN7_Pin) << 7;
  *buttons_state |= HAL_GPIO_ReadPin(BTN8_GPIO_Port, BTN8_Pin) << 8;
  *buttons_state |= HAL_GPIO_ReadPin(BTN9_GPIO_Port, BTN9_Pin) << 9;
  *buttons_state |= HAL_GPIO_ReadPin(BTN10_GPIO_Port, BTN10_Pin) << 10;
  *buttons_state |= HAL_GPIO_ReadPin(BTN11_GPIO_Port, BTN11_Pin) << 11;
  *buttons_state |= HAL_GPIO_ReadPin(BTN12_GPIO_Port, BTN12_Pin) << 12;
  *buttons_state |= HAL_GPIO_ReadPin(BTN13_GPIO_Port, BTN13_Pin) << 13;
  *buttons_state ^= 0x3FFF;
}

/* USER CODE END 2 */
