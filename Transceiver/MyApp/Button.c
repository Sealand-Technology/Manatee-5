#include "Button.h"

uint16_t joystick_buttons_input;

//static int i = 0;		// 持续转所需要的参数
static int j = 0;

void Button_Init(void)
{
	joystick_buttons_input = ((uint16_t)joystick_buttons_row[1] << 8) | joystick_buttons_row[0];
}

uint16_t Get_Button_Val(void)
{
	return joystick_buttons_input;
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

