#ifndef __joystick_H
#define __joystick_H

#include "main.h"
#include "gpio.h"
#include "stm32f1xx_it.h"
#include <stdio.h>
#include <stdlib.h>

extern uint32_t filter_buf[AXIS_NUM][FILTER_WINDOW_SIZE];
extern uint16_t report_data[AXIS_NUM + BUTTONS_ENABLED];
extern uint16_t last_report_data[AXIS_NUM];
extern uint8_t sticks_state[AXIS_NUM];
extern uint16_t buttons_state;
extern uint16_t ADC_data[ADC_CHANNELS_NUM];
extern uint16_t FilterWindow (uint32_t * p_buf, uint16_t new_val);
extern int8_t sticks_count[AXIS_NUM];

void joystick_init(void);
void joystick_detect(void);

#endif
