#include "Transmit.h"


uint8_t row_data[16];	//前8位存data的高8位
						//后8位存data的低8位

uint8_t data_combine1[8];	//存前4个字节
uint8_t data_combine2[8];	//存后4个字节
uint16_t report_data_combine1[4];	// 前四个字节
uint16_t report_data_combine2[4];	// 后四个字节

uint8_t ButtonVal[8];
uint8_t ButtonVal_High;
uint8_t ButtonVal_Low;

//数据接收格式
CAN_RxHeaderTypeDef RxMsg;
uint8_t RxMsgData[4];

//数据发送格式
//uint8_t TxMsgData[4] = {
///*   Data[4]          */
//  0x11, 0x22, 0x33, 0x44   
//};

//CAN_TxHeaderTypeDef TxMsgHeader = {
///*   StdId     ExtId         IDE             RTR        	DLC         timexxx          */
// 		0x300, 	 0x00000000,   CAN_ID_STD,     CAN_RTR_DATA, 4 ,		DISABLE 
//};


// 发送后四个电压值
	CAN_TxHeaderTypeDef TxMsg_ValHeader_High = {
/*   StdId     ExtId      IDE         RTR       DLC   	TransmitGlobalTime          */
	0x500, 0x00000000, CAN_ID_STD, CAN_RTR_DATA, 8 ,DISABLE 
	};
	
	
// 发送前四个电压值
	CAN_TxHeaderTypeDef TxMsg_ValHeader_Low = {
/*   StdId     ExtId      IDE         RTR       DLC   	TransmitGlobalTime          */
	0x400, 0x00000000, CAN_ID_STD, CAN_RTR_DATA, 8 ,DISABLE 
	};
	
	
// 发送按键值，有效值存在后两位，分别是8位数据
	CAN_TxHeaderTypeDef ButtonVal_Header = {
/*   StdId     ExtId      IDE         RTR       DLC   TransmitGlobalTime          */
	0x300, 0x00000000, CAN_ID_STD, CAN_RTR_DATA, 8 ,DISABLE 
	};

	
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
	if (MyCAN_Transmit(&TxMsg_ValHeader_High,data_combine1) != HAL_OK)
	{
		Error_Handler();
	}	
//	else
//	{ 
//		printf("TxMsg_Val_High is Transmit\r\n"); 
//	}

	// 发送后四个电压值
	if (MyCAN_Transmit(&TxMsg_ValHeader_Low,data_combine2) != HAL_OK)
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
	

	if (MyCAN_Transmit(&ButtonVal_Header,ButtonVal) != HAL_OK)
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

