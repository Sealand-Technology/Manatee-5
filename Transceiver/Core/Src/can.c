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

uint32_t can_id;
uint8_t rx_data_length;
uint8_t rx_data[8];

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

  CAN1_Filter_Init();

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

/**
 * @brief  Initialize CAN1 filter to receive all messages.
 * @retval None
 */
void CAN1_Filter_Init(void)
{
  CAN_FilterTypeDef canFilterConfig;

  // Configure the filter to receive all CAN messages
  canFilterConfig.FilterActivation = ENABLE;
  canFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  canFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
  canFilterConfig.FilterIdHigh = 0x0000;
  canFilterConfig.FilterIdLow = 0x0000;
  canFilterConfig.FilterMaskIdHigh = 0x0000;
  canFilterConfig.FilterMaskIdLow = 0x0000;
  canFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
  canFilterConfig.FilterBank = 0;

  // Initialize the filter
  if (HAL_CAN_ConfigFilter(&hcan, &canFilterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
 * @brief  Transmit a CAN message using CAN1.
 * @param  ID: The standard identifier for the CAN message.
 * @param  Length: Length of the data to be transmitted (0 to 8 bytes).
 * @param  Data: Pointer to the data array to be transmitted.
 * @retval HAL_StatusTypeDef: Status of the transmission (HAL_OK, HAL_ERROR, etc.)
 */
HAL_StatusTypeDef CAN1_Transmit(uint32_t ID, uint8_t Length, uint8_t *Data)
{
  CAN_TxHeaderTypeDef TxHeader;
  uint32_t TxMailbox;

  // Set up CAN transmit message parameters
  TxHeader.StdId = ID;
  TxHeader.ExtId = 0x00;
  TxHeader.IDE = CAN_ID_STD;   // Use standard identifier
  TxHeader.RTR = CAN_RTR_DATA; // Data frame
  TxHeader.DLC = Length;
  TxHeader.TransmitGlobalTime = DISABLE;

  // Transmit the CAN message and return status
  return HAL_CAN_AddTxMessage(&hcan, &TxHeader, Data, &TxMailbox);
}

/**
 * @brief  Receive a CAN message using CAN1.
 * @param  ID: The identifier for the received CAN message (output parameter).
 * @param  Length: Length of the received data (output parameter, 0 to 8 bytes).
 * @param  Data: Pointer to the data array to store the received data.
 * @retval HAL_StatusTypeDef: Status of the reception (HAL_OK, HAL_ERROR, etc.)
 */
HAL_StatusTypeDef CAN1_Receive(uint32_t *ID, uint8_t *Length, uint8_t *Data)
{
  CAN_RxHeaderTypeDef RxHeader;

  // Wait until a new message is received and return status
  HAL_StatusTypeDef status = HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &RxHeader, Data);
  if (status != HAL_OK)
  {
    // Return the status in case of error
    return status;
  }

  // Retrieve the standard identifier
  if (RxHeader.IDE == CAN_ID_STD)
  {
    *ID = RxHeader.StdId;
  }
  else
  {
    // Handling for extended identifiers can be implemented here
  }

  // Retrieve the data length
  *Length = RxHeader.DLC;

  return HAL_OK;
}

/**
 * @brief  Check if there are any CAN messages pending in FIFO0.
 * @retval uint8_t: 1 if there are messages pending, 0 if no messages are pending.
 */
uint8_t CAN1_ReceiveFlag(void)
{
  // Check if there are any messages pending in FIFO0
  if (HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_RX_FIFO0) > 0)
  {
    return 1; // Message is pending
  }
  return 0; // No message is pending
}

/**
 * @brief  Callback function for CAN1 receive interrupt.
 * @param  hcan: Pointer to a CAN_HandleTypeDef structure that contains
 *               the configuration information for the specified CAN.
 * @retval None
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  /* Check if the interrupt is triggered by CAN1 */
  if (hcan->Instance == CAN1)
  {
    /* Call CAN1_Receive to get the message */
    if (CAN1_Receive(&can_id, &rx_data_length, rx_data) == HAL_OK)
    {
      /* Successfully received a message */
      if (can_id == 0x300U && rx_data_length == 8)
      {
        // Process the received data as the first four stick values
        for (uint8_t i = 0; i < 4; i++)
        {
          js_axes_in[i] = (int16_t)((rx_data[2 * i] << 8) | rx_data[2 * i + 1]);
        }

        // Set bit 0 indicating first set of joystick axes received
        js_in_status |= 0x01;
      }
      else if (can_id == 0x301U && rx_data_length == 8)
      {
        // Process the received data as the last four stick values
        for (uint8_t i = 0; i < 4; i++)
        {
          js_axes_in[4 + i] = (int16_t)((rx_data[2 * i] << 8) | rx_data[2 * i + 1]);
        }

        // Set bit 1 indicating second set of joystick axes received
        js_in_status |= 0x02;
      }
      else if (can_id == 0x302U && rx_data_length == 2)
      {
        // Process the received data as 16 buttons input (only use the first 2 bytes)
        js_buttons_in = (uint16_t)((rx_data[0] << 8) | rx_data[1]);

        // Set bit 2 indicating buttons data received
        js_in_status |= 0x04;
      }

      /* Check if all joystick axes and buttons data are received */
      if ((js_in_status & 0x07) == 0x07)
      {
        // Send manual control message to ArduSub
        //FMU_Send_Manual_Control_Msg(MAVLINK_COMM_0, js_axes_in, &js_buttons_in);

        //js_buttons_handle();

        // Reset status after processing
        js_in_status = 0;
      }
    }
  }
}

/* USER CODE END 1 */
