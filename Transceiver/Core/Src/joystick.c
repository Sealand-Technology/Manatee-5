#include "joystick.h"

// Status to track data reception (bit 0: first 4 joystick values, bit 1: last 4 joystick values, bit 2: buttons)
volatile uint8_t js_in_status = 0;

int16_t js_axes_in[8];
int16_t js_axes_trim_offset[8] = {0, 0, 0, 0, 0, 0, 0, 0};
uint16_t js_buttons_in;
uint8_t joystick_buttons_row[8];

static int j = 0;

void Button_Init(void)
{
	js_buttons_in = ((uint16_t)joystick_buttons_row[1] << 8) | joystick_buttons_row[0];
}

uint16_t Get_Button_Val(void)
{
	return js_buttons_in;
}

void Button_detect(uint16_t	Button_Val)
{
	for (int i = 0;i <= 3;i++) 
		{
			// 使用位运算获取每一位
			uint16_t bit = ((Button_Val >> i) & 0x01);
//			printf("%u\r\n", bit);
			if(bit)
			{
				printf("reveive the KEY%d signal\r\n" , i); 				
			}
		}
		
		
		if(Button_Val & 0x01)	// 按键1，正转粗调10°
		{
			if(j>80)
				j+=4;
			else
				j+=10;
			if((j++)>84)j=84;
			Motor_SetTargetPosition(&Motor9, j);		//正转粗调10°，转90°：84
			printf("KEY0 Set Position %d\r\n",j);
		}
		
		if(Button_Val & 0x02)	// 按键2，正转微调1°
		{
			j+=1;
			if(j>84)j=84;
			Motor_SetTargetPosition(&Motor9, j);		//正转微调1°，转90°：84
			printf("KEY1 Set Position %d\r\n",j);			
		}
		
		if(Button_Val & 0x04)	// 按键3，反转粗调10°
		{
			j-=10;
			if(j<=0)j=0;
			Motor_SetTargetPosition(&Motor9, j);		//反转粗调10°，转90°：84
			printf("KEY2 Set Position %d\r\n",j);
		}
		
		if(Button_Val & 0x08)	// 按键4，反转微调1°
		{
			j--;
			if(j<=0)j=0;
			Motor_SetTargetPosition(&Motor9, j);		//反转微调1°，转90°：84
			printf("KEY3 Set Position %d\r\n",j);			
		}
}