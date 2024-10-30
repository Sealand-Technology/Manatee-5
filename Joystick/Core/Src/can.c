/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    can.c
  * @brief   This file provides code for the configuration
  *          of the CAN instances.
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
#include "can.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

CAN_HandleTypeDef hcan;

/* CAN init function */
void MX_CAN_Init(void)
{

  /* USER CODE BEGIN CAN_Init 0 */

  /* USER CODE END CAN_Init 0 */

  /* USER CODE BEGIN CAN_Init 1 */

  /* USER CODE END CAN_Init 1 */
  hcan.Instance = CAN1;
  hcan.Init.Prescaler = 18;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_3TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_4TQ;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = DISABLE;
  hcan.Init.AutoWakeUp = DISABLE;
  hcan.Init.AutoRetransmission = DISABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN_Init 2 */

  /* USER CODE END CAN_Init 2 */

}

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */

  /* USER CODE END CAN1_MspInit 0 */
    /* CAN1 clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**CAN GPIO Configuration
    PA11     ------> CAN_RX
    PA12     ------> CAN_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* CAN1 interrupt Init */
    HAL_NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
  /* USER CODE BEGIN CAN1_MspInit 1 */

  /* USER CODE END CAN1_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();

    /**CAN GPIO Configuration
    PA11     ------> CAN_RX
    PA12     ------> CAN_TX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11|GPIO_PIN_12);

    /* CAN1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USB_LP_CAN1_RX0_IRQn);
  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

HAL_StatusTypeDef Init_Filter(void)
{
	CAN_FilterTypeDef CAN_FilterInitStructure;
	CAN_FilterInitStructure.FilterBank=0;//???��????��a3?��??����?1y???�¡�?1y???�¡���12????13????
	CAN_FilterInitStructure.FilterScale=CAN_FILTERSCALE_32BIT;
	CAN_FilterInitStructure.FilterMode=CAN_FILTERMODE_IDMASK;
	CAN_FilterInitStructure.FilterFIFOAssignment=CAN_FILTER_FIFO0;
	CAN_FilterInitStructure.FilterIdHigh=0x600 << 5;
	CAN_FilterInitStructure.FilterIdLow=0x607 << 5;
	CAN_FilterInitStructure.FilterMaskIdHigh=0xFFFF;
	CAN_FilterInitStructure.FilterMaskIdLow=0xFFFF;
	CAN_FilterInitStructure.FilterActivation=CAN_FILTER_ENABLE;
	 return	HAL_CAN_ConfigFilter(&hcan, &CAN_FilterInitStructure);
}

HAL_StatusTypeDef MyCAN_Transmit(CAN_TxHeaderTypeDef *TxMessage, uint8_t *Data) {
	uint32_t pTxMailbox;
	return HAL_CAN_AddTxMessage(&hcan, TxMessage, Data, &pTxMailbox);
}

uint8_t MyCAN_ReceiveFlag(void)
{
	uint32_t aaa=HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_FILTER_FIFO0);
	printf("aaa=%d\n", aaa);
	if (HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_FILTER_FIFO0) > 0)
	{
		return 1;
	}
	return 0;
}

void MyCAN_Receive(CAN_RxHeaderTypeDef *RxMessage, uint8_t *Data) {
	HAL_CAN_GetRxMessage(&hcan, CAN_FILTER_FIFO0, RxMessage, Data); 
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
//	if( HAL_CAN_GetRxMessage(hcan, CAN_FILTER_FIFO0, &RxMsg, RxMsgData)==HAL_OK)
//	{
//		Receive_Flag = 1;
//	}
}

/* USER CODE END 1 */
