#ifndef __Transmit_H
#define __Transmit_H

#include "main.h"
#include "gpio.h"
#include "stm32f1xx_it.h"
#include "can.h"
#include "joystick.h"

void Transmit(void);
void Transmit_detect(void);

#endif
