#include "motor_driver.h"

Motor_t Motor9;
Motor_t Motor10;
Motor_t Motor11;

/**
 * @brief  Initializes the motors.
 * @retval None
 */
void Motors_Init(void)
{
  /* Motor9 Configuration */
  Motor9.motorID = 0x09;
  Motor9.status = MOTOR_STATUS_READY;

  // printf("Motor9 Initing.\r\n");

  Motor_SetControlType(&Motor9, MOTOR_CONTROL_TYPE_POSITION);
  HAL_Delay(100);

  Motor_SetAccelerationUnit(&Motor9, MOTOR_ACCELERATION_UNIT_RAD_S2);
  HAL_Delay(100);

  Motor_SetSpeedUnit(&Motor9, MOTOR_SPEED_UNIT_RPM);
  HAL_Delay(100);

  Motor_SetMaxSpeed(&Motor9, 3510);
  HAL_Delay(100);

  Motor_SetSpeed(&Motor9, 702);
  HAL_Delay(100);

  Motor_SetPositionType(&Motor9, MOTOR_POSITION_TYPE_ABSOLUTE);
  HAL_Delay(100);

  // printf("Motor9 Inited.\r\n");

  // /* Motor10 Configuration */
  // Motor10.motorID = 0x0A;
  // Motor10.status = MOTOR_STATUS_READY;

  // Motor_SetControlType(&Motor10, MOTOR_CONTROL_TYPE_POSITION);

  // Motor_SetAccelerationUnit(&Motor10, MOTOR_ACCELERATION_UNIT_RAD_S2);

  // Motor_SetSpeedUnit(&Motor10, MOTOR_SPEED_UNIT_RPM);

  // Motor_SetMaxSpeed(&Motor10, 5000);

  // Motor_SetSpeed(&Motor10, 3000);

  // Motor_SetPositionType(&Motor10, MOTOR_POSITION_TYPE_ABSOLUTE);

  // Motor_ResetPosition(&Motor10);
  
  // Motor10.targetPosition = 0;
  
    // /* Motor11 Configuration */
  // Motor11.motorID = 0x0B;
  // Motor11.status = MOTOR_STATUS_READY;

  // Motor_SetControlType(&Motor11, MOTOR_CONTROL_TYPE_POSITION);

  // Motor_SetAccelerationUnit(&Motor11, MOTOR_ACCELERATION_UNIT_RAD_S2);

  // Motor_SetSpeedUnit(&Motor11, MOTOR_SPEED_UNIT_RPM);

  // Motor_SetMaxSpeed(&Motor11, 5000);

  // Motor_SetSpeed(&Motor11, 3000);

  // Motor_SetPositionType(&Motor11, MOTOR_POSITION_TYPE_ABSOLUTE);

  // Motor_ResetPosition(&Motor11);
  
  // Motor11.targetPosition = 0;
}

/**************************** AQMD6008BLS-TE WRITE FUNCTIONS *************************************/

/**
 * @brief  Saves all parameters of the motor.
 * @param  motor: Pointer to the Motor object.
 * @retval None
 */
void Motor_SaveAllParameters(Motor_t *motor)
{
  uint32_t canID = 0x600 + motor->motorID;
  uint8_t data[8] = {0x23, 0x10, 0x10, 0x01,
                     0x73, 0x61, 0x76, 0x65};

  motor->status = MOTOR_STATUS_BUSY;
  CAN1_Transmit(canID, 8, data);

  motor->status = MOTOR_STATUS_READY;
}

/**
 * @brief  Sets the control type of the motor.
 * @param  motor: Pointer to the Motor object.
 * @param  controlType: Type of motor control.
 * @retval None
 */
void Motor_SetControlType(Motor_t *motor, uint8_t controlType)
{
  uint32_t canID = 0x600 + motor->motorID;
  uint8_t data[8] = {0x2F, 0x00, 0x20, 0x00,
                     controlType, 0x00, 0x00, 0x00};

  motor->controlType = controlType;
  motor->status = MOTOR_STATUS_BUSY;
  CAN1_Transmit(canID, 8, data);

  motor->status = MOTOR_STATUS_READY;
}

/**
 * @brief  Sets the acceleration unit of the motor.
 * @param  motor: Pointer to the Motor object.
 * @param  accelerationUnit: Acceleration unit (0: Hz/s, 1: Rad/s²).
 * @retval None
 */
void Motor_SetAccelerationUnit(Motor_t *motor, uint8_t accelerationUnit)
{
  uint32_t canID = 0x600 + motor->motorID;
  uint8_t data[8] = {0x2F, 0x0B, 0x20, 0x00,
                     accelerationUnit, 0x00, 0x00, 0x00};

  motor->accelerationUnit = accelerationUnit;
  motor->status = MOTOR_STATUS_BUSY;
  CAN1_Transmit(canID, 8, data);

  motor->status = MOTOR_STATUS_READY;
}

/**
 * @brief  Sets the speed unit of the motor.
 * @param  motor: Pointer to the Motor object.
 * @param  speedUnit: Speed unit (0: Hz, 1: RPM).
 * @retval None
 */
void Motor_SetSpeedUnit(Motor_t *motor, uint8_t speedUnit)
{
  uint32_t canID = 0x600 + motor->motorID;
  uint8_t data[8] = {0x2F, 0x0A, 0x20, 0x00,
                     speedUnit, 0x00, 0x00, 0x00};

  motor->speedUnit = speedUnit;
  motor->status = MOTOR_STATUS_BUSY;
  CAN1_Transmit(canID, 8, data);

  motor->status = MOTOR_STATUS_READY;
}

/**
 * @brief  Sets the maximum speed of the motor.
 * @param  motor: Pointer to the Motor object.
 * @param  maxSpeed: Maximum speed value (1~65535) in RPM.
 * @retval None
 */
void Motor_SetMaxSpeed(Motor_t *motor, uint16_t maxSpeed)
{
  uint32_t canID = 0x600 + motor->motorID;
  uint8_t data[8] = {0x23, 0x08, 0x20, 0x00,
                     (maxSpeed >> 0) & 0xFF,
                     (maxSpeed >> 8) & 0xFF,
                     0x00,
                     0x00};

  motor->maxSpeed = maxSpeed;
  motor->status = MOTOR_STATUS_BUSY;
  CAN1_Transmit(canID, 8, data);

  motor->status = MOTOR_STATUS_READY;
}

/**
 * @brief  Sets the speed of the motor.
 * @param  motor: Pointer to the Motor object.
 * @param  speed: Speed value.
 * @retval None
 */
void Motor_SetSpeed(Motor_t *motor, int32_t speed)
{
  uint32_t canID = 0x600 + motor->motorID;
  uint8_t data[8] = {0x23, 0x01, 0x20, 0x00,
                     (speed >> 0) & 0xFF,
                     (speed >> 8) & 0xFF,
                     (speed >> 16) & 0xFF,
                     (speed >> 24) & 0xFF};

  motor->speed = speed;
  motor->status = MOTOR_STATUS_BUSY;
  CAN1_Transmit(canID, 8, data);

  motor->status = MOTOR_STATUS_READY;
}

/**
 * @brief  Sets the position type of the motor.
 * @param  motor: Pointer to the Motor object.
 * @param  positionType: Position type (0: Absolute, 1: Relative).
 * @retval None
 */
void Motor_SetPositionType(Motor_t *motor, uint8_t positionType)
{
  uint32_t canID = 0x600 + motor->motorID;
  uint8_t data[8] = {0x2F, 0x02, 0x20, 0x00,
                     positionType, 0x00, 0x00, 0x00};

  motor->positionType = positionType;
  motor->status = MOTOR_STATUS_BUSY;
  CAN1_Transmit(canID, 8, data);

  motor->status = MOTOR_STATUS_READY;
}

/**
 * @brief  Resets the position of the motor.
 * @param  motor: Pointer to the Motor object.
 * @retval None
 */
void Motor_ResetPosition(Motor_t *motor)
{
  uint32_t canID = 0x600 + motor->motorID;
  uint8_t data[8] = {0x23, 0x0F, 0x20, 0x00,
                     0x00, 0x00, 0x00, 0x00};

  motor->position = 0;
  motor->status = MOTOR_STATUS_BUSY;
  CAN1_Transmit(canID, 8, data);

  motor->status = MOTOR_STATUS_READY;
}

/**
 * @brief  Sets the target position of the motor.
 * @param  motor: Pointer to the Motor object.
 * @param  targetPosition: Target position value.
 * @retval None
 */
void Motor_SetTargetPosition(Motor_t *motor, int32_t targetPosition)
{
  uint32_t canID = 0x600 + motor->motorID;
  uint8_t data[8] = {0x23, 0x03, 0x20, 0x00,
                     (targetPosition >> 0) & 0xFF,
                     (targetPosition >> 8) & 0xFF,
                     (targetPosition >> 16) & 0xFF,
                     (targetPosition >> 24) & 0xFF};

  motor->status = MOTOR_STATUS_BUSY;
  CAN1_Transmit(canID, 8, data);

  motor->status = MOTOR_STATUS_READY;
}

/*************************** AQMD6008BLS-TE READ FUNCTIONS ***************************************/

/**
 * @brief  Reads the fault status of the motor.
 * @param  motor: Pointer to the Motor object.
 * @retval None
 */
void Motor_ReadFaultStatus(Motor_t *motor)
{
  uint32_t canID = 0x600 + motor->motorID;
  uint8_t data[8] = {0x40, 0x12, 0x21, 0x00,
                     0x00, 0x00, 0x00, 0x00};

  motor->status = MOTOR_STATUS_BUSY;
  CAN1_Transmit(canID, 8, data);

  motor->status = MOTOR_STATUS_READY;
}

/**
 * @brief  Reads the locked rotor status of the motor.
 * @param  motor: Pointer to the Motor object.
 * @retval None
 */
void Motor_ReadLockedRotorStatus(Motor_t *motor)
{
  uint32_t canID = 0x600 + motor->motorID;
  uint8_t data[8] = {0x40, 0x11, 0x21, 0x00,
                     0x00, 0x00, 0x00, 0x00};

  motor->status = MOTOR_STATUS_BUSY;
  CAN1_Transmit(canID, 8, data);

  motor->status = MOTOR_STATUS_READY;
}

/**
 * @brief  Reads the speed of the motor.
 * @param  motor: Pointer to the Motor object.
 * @retval None
 */
void Motor_ReadSpeed(Motor_t *motor)
{
  uint32_t canID = 0x600 + motor->motorID;
  uint8_t data[8] = {0x40, 0x0A, 0x21, 0x00,
                     0x00, 0x00, 0x00, 0x00};

  motor->status = MOTOR_STATUS_BUSY;
  CAN1_Transmit(canID, 8, data);

  motor->status = MOTOR_STATUS_READY;
}

/**
 * @brief  Reads the absolute position of the motor.
 * @param  motor: Pointer to the Motor object.
 * @retval None
 */
void Motor_ReadAbsolutePosition(Motor_t *motor)
{
  uint32_t canID = 0x600 + motor->motorID;
  uint8_t data[8] = {0x40, 0x05, 0x21, 0x00,
                     0x00, 0x00, 0x00, 0x00};

  motor->status = MOTOR_STATUS_BUSY;
  CAN1_Transmit(canID, 8, data);

  motor->status = MOTOR_STATUS_READY;
}

// /**
//  * @brief  Reads the relative position of the motor.
//  * @param  motor: Pointer to the Motor object.
//  * @retval None
//  */
// void Motor_ReadRelativePosition(Motor_t *motor)
// {
//   uint32_t canID = 0x600 + motor->motorID;
//   uint8_t data[8] = {0x40, 0x09, 0x21, 0x00,
//                      0x00, 0x00, 0x00, 0x00};

//   motor->status = MOTOR_STATUS_BUSY;
//   CAN1_Transmit(canID, 8, data);

//   motor->status = MOTOR_STATUS_READY;
// }

// /**
//  * @brief  Reads the position control status of the motor.
//  * @param  motor: Pointer to the Motor object.
//  * @retval None
//  */
// void Motor_ReadPositionControlStatus(Motor_t *motor)
// {
//   uint32_t canID = 0x600 + motor->motorID;
//   uint8_t data[8] = {0x40, 0x06, 0x21, 0x00,
//                      0x00, 0x00, 0x00, 0x00};

//   motor->status = MOTOR_STATUS_BUSY;
//   CAN1_Transmit(canID, 8, data);

//   motor->status = MOTOR_STATUS_READY;
// }

// /**
//  * @brief  Reads the remaining time for position control completion.
//  * @param  motor: Pointer to the Motor object.
//  * @retval None
//  */
// void Motor_ReadPositionControlRemainingTime(Motor_t *motor)
// {
//   uint32_t canID = 0x600 + motor->motorID;
//   uint8_t data[8] = {0x40, 0x0B, 0x21, 0x00,
//                      0x00, 0x00, 0x00, 0x00};

//   motor->status = MOTOR_STATUS_BUSY;
//   CAN1_Transmit(canID, 8, data);

//   motor->status = MOTOR_STATUS_READY;
// }

/******************* AQMD6008BLS-TE DATA FEEDBACK HANDLE FUNCTIONS *******************************/

/**
 * @brief  Handles feedback from AQMD6008BLS-TE.
 * @param  motor: Pointer to the Motor object.
 * @param  feedback_data: Pointer to the received feedback data from CAN.
 * @retval None
 */
void Motor_Feedback_Handler(Motor_t *motor, uint8_t *feedback_data)
{
  switch (feedback_data[0])
  {
  case 0x60:
    Motor_WriteFeedback_Handler(motor, feedback_data);
    break;
  case 0x4F:
    Motor_ReadFeedback_Handler(motor, feedback_data, 1);
    break;
  case 0x4B:
    Motor_ReadFeedback_Handler(motor, feedback_data, 2);
    break;
  case 0x47:
    Motor_ReadFeedback_Handler(motor, feedback_data, 3);
    break;
  case 0x43:
    Motor_ReadFeedback_Handler(motor, feedback_data, 4);
    break;
  case 0x80:
    Motor_AbortFeedback_Handler(motor, feedback_data);
    break;
  default:
    motor->status = MOTOR_STATUS_ERROR;
    // printf("CAN1 received the feedback data with wrong status code.\r\n");
    break;
  }
}

/**
 * @brief  Handles feedback for write functions.
 * @param  motor: Pointer to the Motor object.
 * @param  feedback_data: Pointer to the received feedback data from CAN.
 * @retval None
 */
void Motor_WriteFeedback_Handler(Motor_t *motor, uint8_t *feedback_data)
{
  uint16_t index = ((uint16_t)(feedback_data[2] << 8)) | (uint16_t)(feedback_data[1]);
  switch (index)
  {
  case 0x1010:
    if (feedback_data[3] == 0x01)
    {
      motor->status = MOTOR_STATUS_READY;
      // printf("All parameters were saved successfully.\r\n");
    }
    break;
  case 0x2000:
    motor->status = MOTOR_STATUS_READY;
    // printf("Motor control type was successfully set.\r\n");
    break;
  case 0x2001:
    motor->status = MOTOR_STATUS_READY;
    // printf("Motor control quantity was successfully set.\r\n");
    break;
  case 0x2002:
    motor->status = MOTOR_STATUS_READY;
    // printf("Motor position type was successfully set.\r\n");
    break;
  case 0x2003:
    motor->status = MOTOR_STATUS_READY;
    // printf("Motor target position was successfully set.\r\n");
    break;
  // case 0x2004:
  //   /* code */
  //   break;
  // case 0x2005:
  //   /* code */
  //   break;
  // case 0x2006:
  //   /* code */
  //   break;
  // case 0x2007:
  //   /* code */
  //   break;
  case 0x2008:
    motor->status = MOTOR_STATUS_READY;
    // printf("Motor maximum speed was successfully set.\r\n");
    break;
  case 0x200A:
    motor->status = MOTOR_STATUS_READY;
    // printf("Motor speed unit was successfully set.\r\n");
    break;
  case 0x200B:
    motor->status = MOTOR_STATUS_READY;
    // printf("Motor acceleration unit was successfully set.\r\n");
    break;
  case 0x200F:
    motor->status = MOTOR_STATUS_READY;
    // printf("Motor position count was successfully reset.\r\n");
    break;
  default:
    motor->status = MOTOR_STATUS_ERROR;
    // printf("CAN1 received the write feedback data with wrong dictionary index.\r\n");
    break;
  }
}

/**
 * @brief  Handles feedback for read functions.
 * @param  motor: Pointer to the Motor object.
 * @param  feedback_data: Pointer to the received feedback data from CAN.
 * @param  effective_data_length: Length of effective data.
 * @retval None
 */
void Motor_ReadFeedback_Handler(Motor_t *motor, uint8_t *feedback_data, uint8_t effective_data_length)
{
  uint16_t index = ((uint16_t)(feedback_data[2] << 8)) | (uint16_t)(feedback_data[1]);
  uint32_t data = 0U;
  for (uint8_t i = 0; i < effective_data_length; i++)
  {
    data |= ((uint32_t)(feedback_data[4 + i])) << (8 * i);
  }

  switch (index)
  {
  // case 0x2100:
  //   /* code */
  //   break;
  // case 0x2101:
  //   /* code */
  //   break;
  // case 0x2102:
  //   /* code */
  //   break;
  case 0x2105:
    motor->position = (int32_t)data;
    motor->status = MOTOR_STATUS_READY;
    // printf("Motor absolute position = %d\r\n", motor->position);
    break;
  // case 0x2106:
  //   /* code */
  //   break;
  // case 0x2109:
  //   motor->position = (int32_t)data;
  //   motor->status = MOTOR_STATUS_READY;
  //   printf("Motor relative position = %d\r\n", motor->position);
  //   break;
  case 0x210A:
    motor->speed = (int32_t)data;
    motor->status = MOTOR_STATUS_READY;
    // printf("Motor speed = %d\r\n", motor->speed);
    break;
  // case 0x210B:
  //   /* code */
  //   break;
  // case 0x210C:
  //   /* code */
  //   break;
  // case 0x210D:
  //   /* code */
  //   break;
  // case 0x210E:
  //   /* code */
  //   break;
  // case 0x210F:
  //   /* code */
  //   break;
  case 0x2111:
    if (data != 0)
    {
      motor->status = MOTOR_STATUS_LOCKED_ROTOR;
    }
    break;
  case 0x2112:
    if (data != 0)
    {
      motor->status = MOTOR_STATUS_FAULT;
    }
    break;
  default:
    motor->status = MOTOR_STATUS_ERROR;
    // printf("CAN1 received the read feedback data with wrong dictionary index.\r\n");
    break;
  }
}

/**
 * @brief  Handles feedback for abort.
 * @param  motor: Pointer to the Motor object.
 * @param  feedback_data: Pointer to the received feedback data from CAN.
 * @retval None
 */
void Motor_AbortFeedback_Handler(Motor_t *motor, uint8_t *feedback_data)
{
  motor->status = MOTOR_STATUS_ERROR;

  /* Print abort code directly */
  printf("Abort Code:0x");
  for (uint8_t i = 7; i >= 4; i--)
  {
    printf("%02X", feedback_data[i]);
  }
  printf("\r\n");
}
