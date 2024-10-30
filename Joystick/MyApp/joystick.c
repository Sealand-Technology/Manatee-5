#include "joystick.h"

#if (AXIS_NUM > 0)
uint32_t filter_buf[AXIS_NUM][FILTER_WINDOW_SIZE];
#endif

uint16_t report_data[AXIS_NUM + BUTTONS_ENABLED];
uint16_t last_report_data[AXIS_NUM];
uint8_t sticks_state[AXIS_NUM];
uint16_t buttons_state;
uint16_t ADC_data[ADC_CHANNELS_NUM];
uint16_t FilterWindow (uint32_t * p_buf, uint16_t new_val);
int8_t sticks_count[AXIS_NUM] = {0, 0, 0, 0, 0, 0, 0, 0};

void joystick_init()
{
	// 记录上一次的 report_data 的值, 默认为居中值
	for(int i=0;i<AXIS_NUM;i++)
	{
		last_report_data[i] = 2050;	
	}
	// 0: 摇杆静止, 1: 摇杆拨动
	for(int i=0;i<AXIS_NUM;i++)
	{
		sticks_state[i] = 0;	
	}	
	// 摇杆数据变化的次数，若计数超过阈值，则sticks_state发生变化
	for(int i=0;i<AXIS_NUM;i++)
	{
		sticks_count[i] = 0;	
	}
	// 初始化按键状态
	buttons_state = 0x0000;
}

void joystick_detect()
{
		GPIO_Poll(&buttons_state);
	
#if (AXIS_NUM > 0)
        for (uint8_t i = 0; i < AXIS_NUM; i++)
        {
//			uint16_t filtered_value = FilterWindow(filter_buf[i], ADC_data[i]);
			uint16_t filtered_value = ADC_data[i];

			int16_t diff = filtered_value - last_report_data[i];
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
					filtered_value = last_report_data[i] + MAX_CHANGE;
				else if (diff < -MAX_CHANGE)
					filtered_value = last_report_data[i] - MAX_CHANGE;
				else
					filtered_value += diff;

				// 查看摇杆数据是否越级
				if (filtered_value > 4096)
					report_data[i] = last_report_data[i];
				else
				{
					report_data[i] = filtered_value;
					last_report_data[i] = filtered_value; // 保存摇杆数据
				}

				// HAL_Delay(5);
			}
			
			else // 若摇杆静止
			{
				filtered_value = last_report_data[i];
				report_data[i] = filtered_value;
			}
		}
#endif
			
#if (BUTTONS_ENABLED == 1)
			report_data[AXIS_NUM] = buttons_state;
			buttons_state = 0x0000;		// 每次得到按键值后清零
#endif  
	  // 反转特定轴的数据
//	  report_data[1] = 4096 - report_data[1];
//	  report_data[4] = 4096 - report_data[4];
//	  report_data[6] = 4096 - report_data[6];
//	  report_data[7] = 4096 - report_data[7];
	
}

uint16_t FilterWindow(uint32_t * p_buf, uint16_t new_val)
{
    uint32_t sum = new_val;

    // 将新值添加到缓冲区末尾
    for (uint8_t i = 1; i < FILTER_WINDOW_SIZE; i++)
    {
        p_buf[i - 1] = p_buf[i];
        sum += p_buf[i - 1];
    }
    p_buf[FILTER_WINDOW_SIZE - 1] = new_val;

    // 计算平均值
    uint16_t ret = sum / FILTER_WINDOW_SIZE;

    // 确保返回值在 [0, 4096] 范围
    if (ret > 4096)
    {
      ret = 4096;
    }
    return ret;
}

