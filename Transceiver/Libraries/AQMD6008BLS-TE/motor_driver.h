/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MOTOR_H__
#define __MOTOR_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include "usart.h"
#include "can.h"

/* Defines ------------------------------------------------------------------*/

// Motor status
#define MOTOR_STATUS_READY 0x00
#define MOTOR_STATUS_ERROR 0x01
#define MOTOR_STATUS_BUSY 0x02
#define MOTOR_STATUS_LOCKED_ROTOR 0x03
#define MOTOR_STATUS_FAULT 0x04

// Motor control type
#define MOTOR_CONTROL_TYPE_PWM 0x00       // Speed control by PWM
#define MOTOR_CONTROL_TYPE_VELOCITY 0x01  // Closed-loop speed control
#define MOTOR_CONTROL_TYPE_TORQUE 0x02    // Torque control
#define MOTOR_CONTROL_TYPE_POSITION 0x03  // Position closed-loop control
#define MOTOR_CONTROL_TYPE_STOP 0x10      // Normal stop
#define MOTOR_CONTROL_TYPE_BRAKE 0x11     // Emergency stop
#define MOTOR_CONTROL_TYPE_FREE_STOP 0x12 // Free stop

// Motor acceleration unit
#define MOTOR_ACCELERATION_UNIT_HZ_S     0x00  // Hz/s
#define MOTOR_ACCELERATION_UNIT_RAD_S2   0x01  // Rad/s²

// Motor speed units
#define MOTOR_SPEED_UNIT_HZ           0x00  // Hz
#define MOTOR_SPEED_UNIT_RPM          0x01  // RPM

// Motor position type
#define MOTOR_POSITION_TYPE_ABSOLUTE  0x00  // Absolute position
#define MOTOR_POSITION_TYPE_RELATIVE  0x01  // Relative position

// Motor structure
typedef struct {
  uint8_t motorID;
  volatile uint8_t status;
  uint8_t controlType;
  uint8_t accelerationUnit;
  uint8_t speedUnit;
  uint16_t maxSpeed;
  int32_t speed;
  uint8_t positionType;
  int32_t position;
  int32_t targetPosition;
}Motor_t;

extern Motor_t Motor9;
extern Motor_t Motor10;
extern Motor_t Motor11;

extern volatile uint8_t control_value;

/* Prototypes ------------------------------------------------------------------*/

void Motors_Init(void);

void Motor_SaveAllParameters(Motor_t *motor);
void Motor_SetControlType(Motor_t *motor, uint8_t controlType);
void Motor_SetAccelerationUnit(Motor_t *motor, uint8_t accelerationUnit);
void Motor_SetSpeedUnit(Motor_t *motor, uint8_t speedUnit);
void Motor_SetMaxSpeed(Motor_t *motor, uint16_t maxSpeed);
void Motor_SetSpeed(Motor_t *motor, int32_t speed);
void Motor_SetPositionType(Motor_t *motor, uint8_t positionType);
void Motor_ResetPosition(Motor_t *motor);
void Motor_SetTargetPosition(Motor_t *motor, int32_t targetPosition);

void Motor_ReadFaultStatus(Motor_t *motor);
void Motor_ReadLockedRotorStatus(Motor_t *motor);
void Motor_ReadSpeed(Motor_t *motor);
void Motor_ReadAbsolutePosition(Motor_t *motor);
// void Motor_ReadRelativePosition(Motor_t *motor);
// void Motor_ReadPositionControlStatus(Motor_t *motor);
// void Motor_ReadPositionControlRemainingTime(Motor_t *motor);

void Motor_Feedback_Handler(Motor_t *motor, uint8_t *feedback_data);
void Motor_WriteFeedback_Handler(Motor_t *motor, uint8_t *feedback_data);
void Motor_ReadFeedback_Handler(Motor_t *motor, uint8_t *feedback_data, uint8_t effective_data_length);
void Motor_AbortFeedback_Handler(Motor_t *motor, uint8_t *feedback_data);


#ifdef __cplusplus
}
#endif

#endif /* __MOTOR_H__ */
