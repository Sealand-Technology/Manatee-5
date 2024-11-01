#include "joystick.h"

uint16_t report_data[AXIS_NUM + BUTTONS_ENABLED];
uint16_t last_report_data[AXIS_NUM];
uint8_t sticks_state[AXIS_NUM];
uint16_t buttons_state;
uint16_t ADC_data[ADC_CHANNEL_NUM];
int8_t sticks_count[AXIS_NUM];

uint8_t tx_data[8];

void JS_Init(void)
{
	// 初始化按键状态
	buttons_state = 0x0000;

	for (uint8_t i = 0; i < AXIS_NUM; i++)
	{
		// 记录上一次的 report_data 的值, 默认为居中值
		last_report_data[i] = 2048;

		// 0: 摇杆静止, 1: 摇杆拨动
		sticks_state[i] = 0;

		// 摇杆数据变化的次数，若计数超过阈值，则sticks_state发生变化
		sticks_count[i] = 0;
	}
}

void JS_Update(void)
{
	GPIO_Poll(&buttons_state);

	for (uint8_t i = 0; i < AXIS_NUM; i++)
	{
		uint16_t adc_value = ADC_data[i];

		int16_t diff = adc_value - last_report_data[i];
		if (abs(diff) < DEAD_ZONE)
		{
			sticks_count[i]--;
			if (sticks_count[i] <= STICK_STATIC_VAL)
			{
				sticks_count[i] = 0;
				sticks_state[i] = 0; // 摇杆静止
			}
		}
		else
		{
			sticks_count[i]++;
			if (sticks_count[i] >= STICK_ACTIVE_VAL)
			{
				sticks_count[i] = 0;
				sticks_state[i] = 1; // 摇杆拨动
			}
		}

		if (sticks_state[i]) // 若摇杆被持续拨动
		{
			// 限制单次变化范围
			if (diff > MAX_CHANGE)
			{
				adc_value = last_report_data[i] + MAX_CHANGE;
			}
			else if (diff < -MAX_CHANGE)
			{
				adc_value = last_report_data[i] - MAX_CHANGE;
			}
			else
			{
				adc_value += diff;
			}

			if (adc_value > 4096)  // 查看摇杆数据是否越级
			{
				report_data[i] = last_report_data[i];
			}
			else
			{
				report_data[i] = adc_value;
				last_report_data[i] = adc_value; // 保存摇杆数据
			}
		}
		else // 若摇杆静止
		{
			adc_value = last_report_data[i];
			report_data[i] = adc_value;
		}
	}

	report_data[AXIS_NUM] = buttons_state;
}

void JS_Transmit(void)
{
	// First set of four uint16_t values: report_data[0] to report_data[3]
	for (uint8_t i = 0; i < 4; i++)
	{
		tx_data[2 * i] = (report_data[i] >> 8) & 0xFF; // Store high byte in tx_data
		tx_data[2 * i + 1] = report_data[i] & 0xFF;		 // Store low byte in tx_data
	}

	// Transmit the voltage value of the first four sticks
	CAN1_Transmit(0x300U, 8, tx_data);

	// Second set of four uint16_t values: report_data[4] to report_data[7]
	for (uint8_t i = 0; i < 4; i++)
	{
		tx_data[2 * i] = (report_data[i + 4] >> 8) & 0xFF; // Store high byte
		tx_data[2 * i + 1] = report_data[i + 4] & 0xFF;		 // Store low byte
	}

	// Transmit the voltage value of the last four sticks
	CAN1_Transmit(0x301U, 8, tx_data);

	// Process the last single uint16_t value, report_data[8]
	tx_data[0] = (report_data[8] >> 8) & 0xFF; // Store high byte
	tx_data[1] = report_data[8] & 0xFF;				 // Store low byte

	// Transmit the status of the buttons
	// 0: Release
	// 1: Press
	CAN1_Transmit(0x302U, 2, tx_data);
}
