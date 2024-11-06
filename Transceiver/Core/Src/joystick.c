#include "joystick.h"

int16_t js_axes_in[8] = {2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048};
int16_t js_axes_trim_offset[8] = {0, 0, 0, 0, 0, 0, 0, 0};
uint16_t js_buttons_in = 0;

uint8_t JS_Buttons_Detect(void)
{
	// Define a mask with bits 3, 10, 11, 12, 13, and 14 set to 1
  uint16_t mask = (1 << 2) | (1 << 9) | (1 << 10) | (1 << 11) | (1 << 12) | (1 << 13);

  // Check if any of the specified bits are 1
  return (js_buttons_in & mask) != 0;
}

void JS_Buttons_Handle(void)
{
	if (js_buttons_in & (1 << 2))
	{
		Motor_SetTargetPosition(&Motor9, 0);
	}
	if (js_buttons_in & (1 << 9)) // Check if bit 10 is set (粗调正转 10°) 7.8 一度
	{
		if (Motor9.position + 78 > 702) // Don't over [0,90] degrees
		{
			Motor_SetTargetPosition(&Motor9, 702);
		}
		else if (Motor9.position + 78 >= 140 && Motor9.position + 78 <= 148)
		{
			// Don't to lie between [18,19] degrees because the mechanics has a dead point
			Motor_SetTargetPosition(&Motor9, Motor9.position + 78 + 8);
		}
		else
		{
			Motor_SetTargetPosition(&Motor9, Motor9.position + 78);
		}
		// printf("BTN10: Set Position\r\n");
	}
	if (js_buttons_in & (1 << 10)) // Check if bit 11 is set (微调正转 1°)
	{
		if (Motor9.position + 8 > 702) // Don't over [0,90] degrees
		{
			Motor_SetTargetPosition(&Motor9, 702);
		}
		else if (Motor9.position + 8 >= 140 && Motor9.position + 8 <= 148)
		{
			// Don't to lie between [18,19] degrees because the mechanics has a dead point
			Motor_SetTargetPosition(&Motor9, Motor9.position + 8 + 8);
		}
		else
		{
			Motor_SetTargetPosition(&Motor9, Motor9.position + 8);
		}
		// printf("BTN11: Set Position\r\n");
	}
	if (js_buttons_in & (1 << 11)) // Check if bit 12 is set (粗调反转 10°)
	{
		if (Motor9.position - 78 < 0) // Don't over [0,90] degrees
		{
			Motor_SetTargetPosition(&Motor9, 0);
		}
		else if (Motor9.position - 78 >= 140 && Motor9.position - 78 <= 148)
		{
			// Don't to lie between [18,19] degrees because the mechanics has a dead point
			Motor_SetTargetPosition(&Motor9, Motor9.position - 78 - 8);
		}
		else
		{
			Motor_SetTargetPosition(&Motor9, Motor9.position - 78);
		}
		// printf("BTN12: Set Position\r\n");
	}
	if (js_buttons_in & (1 << 12)) // Check if bit 13 is set (微调反转 1°)
	{
		if (Motor9.position - 8 < 0) // Don't over [0,90] degrees
		{
			Motor_SetTargetPosition(&Motor9, 0);
		}
		else if (Motor9.position - 8 >= 140 && Motor9.position - 8 <= 148)
		{
			// Don't to lie between [18,19] degrees because the mechanics has a dead point
			Motor_SetTargetPosition(&Motor9, Motor9.position - 8 - 8);
		}
		else
		{
			Motor_SetTargetPosition(&Motor9, Motor9.position - 8);
		}
		// printf("BTN13: Set Position\r\n");
	}
	if (js_buttons_in & (1 << 13)) // Check if bit 14 is set (重置位置为0)
	{
		Motor_ResetPosition(&Motor9);
		// printf("BTN14: Reset Position\r\n");
	}
}
