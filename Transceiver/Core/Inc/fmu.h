/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FMU_H__
#define __FMU_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32_mavlink.h"
#include "Button.h"
#include "can.h"


/* Defines ----------------------------------------------------------------*/

//extern int16_t joystick_axes_input[8];
//extern uint8_t joystick_buttons_row[8];
//extern uint16_t joystick_buttons_input;
	
#define JOYSTICK_AXES_INPUT_MIN       0
#define JOYSTICK_AXES_INPUT_MAX    4096
#define JOYSTICK_AXES_OUTPUT_MIN  -1000
#define JOYSTICK_AXES_OUTPUT_MAX   1000

#define MAP_VALUE(val, in_min, in_max, out_min, out_max) (((val) - (in_min)) * ((out_max) - (out_min)) / ((in_max) - (in_min)) + (out_min))
	
extern volatile uint8_t joystick_input_status;

/* Prototypes ----------------------------------------------------------------*/
void FMU_MAVLink_Init(UART_HandleTypeDef *huart, mavlink_channel_t chan);

void FMU_Send_Reboot_Msg(mavlink_channel_t chan);
void FMU_Send_Arm_Msg(mavlink_channel_t chan);
void FMU_Send_Manual_Control_Msg(mavlink_channel_t chan, int16_t *joystick_axes_input, uint16_t *joystick_buttons_input);

void FMU_Receive_Heartbeat_Msg(void);


#ifdef __cplusplus
}
#endif

#endif /* __FMU_H__ */

