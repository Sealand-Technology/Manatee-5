/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FMU_H__
#define __FMU_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "can.h"
#include "joystick.h"
#include "stm32_mavlink.h"

/* Defines ----------------------------------------------------------------*/
	



/* Prototypes ----------------------------------------------------------------*/

/**
 * @brief  Maps an integer value from one range to another with improved precision.
 *         Uses int32_t for intermediate calculations to reduce precision loss.
 * @param  val: The input value to be mapped.
 * @param  in_min: The minimum value of the input range.
 * @param  in_max: The maximum value of the input range.
 * @param  out_min: The minimum value of the output range.
 * @param  out_max: The maximum value of the output range.
 * @retval int16_t: The mapped value in the output range.
 */
static inline int16_t map_value(int16_t val, int16_t in_min, int16_t in_max, int16_t out_min, int16_t out_max)
{
  return (int32_t)(val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void FMU_MAVLink_Init(UART_HandleTypeDef *huart, mavlink_channel_t chan);

void FMU_Send_Reboot_Msg(mavlink_channel_t chan);
void FMU_Send_Arm_Msg(mavlink_channel_t chan);
void FMU_Send_Manual_Control_Msg(mavlink_channel_t chan, int16_t *js_axes_in, uint16_t *js_buttons_in);

void FMU_Receive_Heartbeat_Msg(void);


#ifdef __cplusplus
}
#endif

#endif /* __FMU_H__ */

