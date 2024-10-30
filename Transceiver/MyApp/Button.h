#ifndef __Button_H
#define __Button_H

//#include "stm32f10x_hal.h"
#include "fmu.h"
#include "can.h"
#include "Motor.h"

extern uint16_t joystick_buttons_input;

void Button_Init(void);
uint16_t Get_Button_Val(void);
void Button_detect(uint16_t	Button_Val);

#endif
