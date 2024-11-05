/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include "motor_driver.h"


/* Defines -------------------------------------------------------------------*/

#define JS_AXES_IN_MIN       0
#define JS_AXES_IN_MAX    4096
#define JS_AXES_OUT_MIN  -1000
#define JS_AXES_OUT_MAX   1000

extern volatile uint8_t js_in_status;
extern int16_t js_axes_in[8];
extern int16_t js_axes_trim_offset[8];
extern uint16_t js_buttons_in;
extern uint8_t joystick_buttons_row[8];


/* Prototypes ----------------------------------------------------------------*/

uint8_t JS_Buttons_Detect(void);
void JS_Buttons_Handle(void);


#ifdef __cplusplus
}
#endif

#endif /* __JOYSTICK_H__ */

