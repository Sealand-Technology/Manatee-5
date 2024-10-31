#include "joystick.h"

uint16_t report_data[AXIS_NUM + BUTTONS_ENABLED];
uint16_t last_report_data[AXIS_NUM];
uint8_t sticks_state[AXIS_NUM];
uint16_t buttons_state;
uint16_t ADC_data[ADC_CHANNEL_NUM];
int8_t sticks_count[AXIS_NUM] = {0, 0, 0, 0, 0, 0, 0, 0};

uint8_t row_data[16]; // 前8位存data的高8位
											// 后8位存data的低8位

uint8_t data_combine1[8];					// 存前4个字节
uint8_t data_combine2[8];					// 存后4个字节
uint16_t report_data_combine1[4]; // 前四个字节
uint16_t report_data_combine2[4]; // 后四个字节

uint8_t ButtonVal[8];
uint8_t ButtonVal_High;
uint8_t ButtonVal_Low;

// 数据接收格式
CAN_RxHeaderTypeDef RxMsg;
uint8_t RxMsgData[4];

// 发送后四个电压值
CAN_TxHeaderTypeDef TxMsg_ValHeader_High;

// 发送前四个电压值
CAN_TxHeaderTypeDef TxMsg_ValHeader_Low;

// 发送按键值，有效值存在后两位，分别是8位数据
CAN_TxHeaderTypeDef ButtonVal_Header;

void joystick_init()
{
	// 初始化按键状态
	buttons_state = 0x0000;

	for (uint8_t i = 0; i < AXIS_NUM; i++)
	{
		// 记录上一次的 report_data 的值, 默认为居中值
		last_report_data[i] = 2050;

		// 0: 摇杆静止, 1: 摇杆拨动
		sticks_state[i] = 0;

		// 摇杆数据变化的次数，若计数超过阈值，则sticks_state发生变化
		sticks_count[i] = 0;
	}
	
	ButtonVal_Header.StdId = 0x300;
	ButtonVal_Header.ExtId = 0x00000000;
	ButtonVal_Header.IDE = CAN_ID_STD;
	ButtonVal_Header.RTR = CAN_RTR_DATA;
	ButtonVal_Header.DLC = 8;
	ButtonVal_Header.TransmitGlobalTime = DISABLE;

	TxMsg_ValHeader_Low.StdId = 0x400;
	TxMsg_ValHeader_Low.ExtId = 0x00000000;
	TxMsg_ValHeader_Low.IDE = CAN_ID_STD;
	TxMsg_ValHeader_Low.RTR = CAN_RTR_DATA;
	TxMsg_ValHeader_Low.DLC = 8;
	TxMsg_ValHeader_Low.TransmitGlobalTime = DISABLE;

	TxMsg_ValHeader_High.StdId = 0x500;
	TxMsg_ValHeader_High.ExtId = 0x00000000;
	TxMsg_ValHeader_High.IDE = CAN_ID_STD;
	TxMsg_ValHeader_High.RTR = CAN_RTR_DATA;
	TxMsg_ValHeader_High.DLC = 8;
	TxMsg_ValHeader_High.TransmitGlobalTime = DISABLE;
}

void joystick_detect()
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

	// 反转特定轴的数据
	//	report_data[1] = 4096 - report_data[1];
	//	report_data[4] = 4096 - report_data[4];
	//	report_data[6] = 4096 - report_data[6];
	//	report_data[7] = 4096 - report_data[7];
}
	
void Transmit(void)
{			
	for(int i = 0; i<8; i++)
	{
		row_data[i] = (uint8_t)(report_data[i] >> 8);
		row_data[i+8] = (uint8_t)(report_data[i] & 0xFF);
	}

	for(int i = 0; i<8; i++)
	{
		if(i<4)
		{
			//前四个数据：0-3  8-11
			// i:0-3
			data_combine1[i] = row_data[i];
			data_combine1[i+4] = row_data[i+8];
		}
		else
		{	
			//后四个数据：4-7  12-15
			// i:4-7
			data_combine2[i-4] = row_data[i];
			data_combine2[i+4-4] = row_data[i+8];
		}
	}			  

	// 发送前四个电压值
	if (CAN1_Transmit(&TxMsg_ValHeader_High,data_combine1) != HAL_OK)
	{
		Error_Handler();
	}	
//	else
//	{ 
//		printf("TxMsg_Val_High is Transmit\r\n"); 
//	}

	// 发送后四个电压值
	if (CAN1_Transmit(&TxMsg_ValHeader_Low,data_combine2) != HAL_OK)
	{
		Error_Handler();
	}	
//	else
//	{ 
//		printf("TxMsg_Val_Low is Transmit\r\n"); 
//	}
	
	ButtonVal_High = (uint8_t)(report_data[AXIS_NUM] >> 8);
	ButtonVal_Low = (uint8_t)(report_data[AXIS_NUM] & 0xFF);

	ButtonVal[0] = ButtonVal_Low;
	ButtonVal[1] = ButtonVal_High;
	

	if (CAN1_Transmit(&ButtonVal_Header,ButtonVal) != HAL_OK)
	{
		Error_Handler();
	}	
//	else
//	{ 
//		printf("TxMsg_ButtonVal is Transmit\r\n"); 
//	}
}

void Transmit_detect(void)
{
//	for(int i = 0; i<8 ; i++)
//	{
//		printf("report_data[%d] = %d\r\n", i,report_data[i]);
//	}

	// 检测8个电压值数据是否正确
	for(int i = 0; i<4 ; i++)
	{
		report_data_combine1[i] = ((uint16_t)data_combine1[i] << 8) | data_combine1[i+4];
		report_data_combine2[i] = ((uint16_t)data_combine2[i] << 8) | data_combine2[i+4];		  
	}	

//	for(int i = 0; i<4 ; i++)
//	{
//	 	printf("report_data_combine1[%d] = %d\r\n", i,report_data_combine1[i]);
//		printf("report_data_combine2[%d] = %d\r\n", i,report_data_combine2[i]);		  
//	}

	// 检测按键值是否对应按键按下
	for (int i = 0;i <= 3;i++) 
	{
		// 使用位运算获取每一位
		uint16_t bit = ((report_data[AXIS_NUM] >> i) & 0x01);
//			printf("%u\r\n", bit);
		if(bit)
		{
			printf("reveive the %d signal\r\n" , i); 				
		}
	}
	
	// 检测按键值数据是否正确
//	printf("buttons_state = 0x%04x",buttons_state);
//	printf("report_data[8] = 0x%04x", report_data[AXIS_NUM]);
//	printf("ButtonVal[0] = 0x%02x", ButtonVal[0]);
//	printf("ButtonVal[1] = 0x%02x", ButtonVal[1]);	
}
