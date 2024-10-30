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

uint8_t KEY0_PressFlag;
uint8_t KEY1_PressFlag;
uint8_t KEY2_PressFlag;
uint8_t KEY3_PressFlag;

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

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED1_Pin|LED2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PBPin PBPin */
  GPIO_InitStruct.Pin = LED1_Pin|LED2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin PBPin PBPin */
  GPIO_InitStruct.Pin = KEY0_Pin|KEY1_Pin|KEY2_Pin|KEY3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PAPin PAPin PAPin */
  GPIO_InitStruct.Pin = BTN5_Pin|BTN6_Pin|BTN7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin PBPin PBPin
                           PBPin PBPin PBPin */
  GPIO_InitStruct.Pin = BTN8_Pin|BTN9_Pin|BTN10_Pin|BTN11_Pin
                          |BTN12_Pin|BTN13_Pin|BTN14_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 2 */

void GPIO_Poll(uint16_t *p_button_state)
{
	//  *p_button_state = 0x0000;
	
	//  *p_button_state ^= 0x3FFF;	因为这句，所以逻辑需要反着来
	if (!KEY0_PressFlag) {
        *p_button_state &= ~(1 << 0);  // 如果标志位为1，则设置第0位，
    } else {
		KEY0_PressFlag = 0;
        *p_button_state |= (1 << 0); // 如果标志位为0，则清除第0位
    }
	if (!KEY1_PressFlag) {
        *p_button_state &= ~(1 << 1);// 如果标志位为1，则设置第1位
    } else {
		KEY1_PressFlag = 0;
        *p_button_state |= (1 << 1); // 如果标志位为0，则清除第1位
    }
	if (!KEY2_PressFlag) {
        *p_button_state &= ~(1 << 2); // 如果标志位为1，则设置第2位
    } else {
		KEY2_PressFlag = 0;		
        *p_button_state |= (1 << 2); // 如果标志位为0，则清除第2位
    }
	if (!KEY3_PressFlag) {
        *p_button_state &= ~(1 << 3);// 如果标志位为1，则设置第3位
    } else {
		KEY3_PressFlag = 0;		
        *p_button_state |= (1 << 3); // 如果标志位为0，则清除第3位
    }	
  *p_button_state |= HAL_GPIO_ReadPin(BTN5_GPIO_Port, BTN5_Pin) << 4;
  *p_button_state |= HAL_GPIO_ReadPin(BTN6_GPIO_Port, BTN6_Pin) << 5;
  *p_button_state |= HAL_GPIO_ReadPin(BTN7_GPIO_Port, BTN7_Pin) << 6;
  *p_button_state |= HAL_GPIO_ReadPin(BTN8_GPIO_Port, BTN8_Pin) << 7;
  *p_button_state |= HAL_GPIO_ReadPin(BTN9_GPIO_Port, BTN9_Pin) << 8;
  *p_button_state |= HAL_GPIO_ReadPin(BTN10_GPIO_Port, BTN10_Pin) << 9;
  *p_button_state |= HAL_GPIO_ReadPin(BTN11_GPIO_Port, BTN11_Pin) << 10;
  *p_button_state |= HAL_GPIO_ReadPin(BTN12_GPIO_Port, BTN12_Pin) << 11;
  *p_button_state |= HAL_GPIO_ReadPin(BTN13_GPIO_Port, BTN13_Pin) << 12;
  *p_button_state |= HAL_GPIO_ReadPin(BTN14_GPIO_Port, BTN14_Pin) << 13;
  *p_button_state ^= 0x3FF0;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
 {
	if(GPIO_Pin == KEY0_Pin)
	{
		KEY0_PressFlag = 1;
	}
	
	if(GPIO_Pin == KEY1_Pin)
	{
		KEY1_PressFlag = 1;
	}
	
 	if(GPIO_Pin == KEY2_Pin)
	{
		KEY2_PressFlag = 1;		
	}
	
	if(GPIO_Pin == KEY3_Pin)
	{
		KEY3_PressFlag = 1;		
	}
 }
 
// uint8_t Get_KEY0_Flag(void)
//{
//	if(KEY0_PressFlag == 1)
//	{
//		KEY0_PressFlag = 0;
//		return 1;
//	}
//	return 0;
//}

/* USER CODE END 2 */
