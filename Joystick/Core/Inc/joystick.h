/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include <stdlib.h>

#include "main.h"
#include "gpio.h"
#include "can.h"
#include "usart.h"

/* Defines -------------------------------------------------------------------*/

extern uint16_t report_data[AXIS_NUM + BUTTONS_ENABLED];
extern uint16_t last_report_data[AXIS_NUM];
extern uint8_t sticks_state[AXIS_NUM];
extern uint16_t buttons_state;
extern uint16_t ADC_data[ADC_CHANNEL_NUM];
extern int8_t sticks_count[AXIS_NUM];

/* Prototypes ----------------------------------------------------------------*/

void JS_Init(void);
void JS_Update(void);
void JS_Transmit(void);

#ifdef __cplusplus
}
#endif

#endif /* __JOYSTICK_H__ */
