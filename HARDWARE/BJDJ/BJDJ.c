#include "BJDJ.h"

void BJDJ_Init(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOF, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOD, GPIO_Pin_6|GPIO_Pin_7);
	GPIO_ResetBits(GPIOE, GPIO_Pin_5|GPIO_Pin_6);
	
}

void motor_step1( )
{
			 PDout(6)=1;
			 PDout(7)=0;
			 PEout(5)=0;
			 PEout(6)=0;
}

void motor_step2( )
{
			 PDout(6)=0;
			 PDout(7)=1;
			 PEout(5)=0;
			 PEout(6)=0;
}
void motor_step3( )
{
	     PDout(6)=0;
			 PDout(7)=0;
			 PEout(5)=1;
			 PEout(6)=0;
}
void motor_step4( )
{
			 PDout(6)=0;
			 PDout(7)=0;
			 PEout(5)=0;
			 PEout(6)=1;
}

void CW_motor()
{
	  motor_step4();
		delay_us(2500);
		motor_step3();
		delay_us(2500);
		motor_step2();
	  delay_us(2500);
		motor_step1();
		delay_us(2500);
}

void ANTICW_motor()
{
	  motor_step1();
		delay_us(2500);
		motor_step2();
		delay_us(2500);
		motor_step3();
		delay_us(2500);
		motor_step4();
		delay_us(2500);
}
