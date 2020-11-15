#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "BJDJ.h"
#include "adc.h"
//#include "lcd.h"
#include "key.h"
#include "exti.h"
#include "timer.h"

//ALIENTEK 探索者STM32F407开发板 实验1
//跑马灯实验 -库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com  
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK
u8 uper_motor_CW_flag=1;
u8 uper_motor_ANTICW_flag=1;

int main(void)
{ 
  u16 adcx;
//	float temp;
	u8 count=0;
	u8 motorflag=0;
	extern u8 return_motor_start_flag;
	extern u8 return_motor_CW_flag;
	extern u8 return_motor_ANTICW_flag;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);    //初始化延时函数
	uart_init(115200);	//初始化串口波特率为115200
	LED_Init();					//初始化LED 
 	//LCD_Init();         //初始化LCD接口
	Adc_Init();         //初始化ADC
	//BEEP_Init();        //初始化蜂鸣器端口
	EXTIX_Init();       //初始化外部中断输入 
	/*POINT_COLOR=RED; 
	LCD_ShowString(30,50,200,16,16,"Explorer STM32F4");	
	LCD_ShowString(30,70,200,16,16,"ADC TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2014/5/6");	  
	POINT_COLOR=BLUE;//设置字体为蓝色
	LCD_ShowString(30,130,200,16,16,"ADC1_CH5_VAL:");	      
	LCD_ShowString(30,150,200,16,16,"ADC1_CH5_VOL:0.000V");	//先在固定位置显示小数点    */ 
	BJDJ_Init();
  TIM3_Int_Init(10-1,8400-1);
	
	while(1)
	{
		adcx=Get_Adc_Average(ADC_Channel_5,20);//获取通道5的转换值，20次取平均
		if(adcx>=3850)
		{
			count++;
			uper_motor_CW_flag=0;
			printf("press high\r\n");
		}
		else
		{
			if(count>=10)
			{
				printf("already\r\n");
				motorflag=1;
			}
			else
			{
				count=0;			
			}
			
			if(motorflag)
			{
				uper_motor_CW_flag=0;
			}
			else
			{
				uper_motor_CW_flag=1;
			}			
		}
		//LCD_ShowxNum(134,130,adcx,4,16,0);    //显示ADCC采样后的原始值
		//temp=(float)adcx*(3.3/4096);          //获取计算后的带小数的实际电压值，比如3.1111
		//adcx=temp;                            //赋值整数部分给adcx变量，因为adcx为u16整形
		//LCD_ShowxNum(134,150,adcx,1,16,0);    //显示电压值的整数部分，3.1111的话，这里就是显示3
		//temp-=adcx;                           //把已经显示的整数部分去掉，留下小数部分，比如3.1111-3=0.1111
		//temp*=1000;                           //小数部分乘以1000，例如：0.1111就转换为111.1，相当于保留三位小数。
		//LCD_ShowxNum(150,150,temp,3,16,0X80); //显示小数部分（前面转换为了整形显示），这里显示的就是111.
		//LED0=!LED0;
		delay_ms(250);	
		//printf("%d %d %d\r\n",return_motor_start_flag,return_motor_CW_flag,uper_motor_CW_flag);
		if(return_motor_start_flag&&return_motor_CW_flag&&uper_motor_CW_flag)
		{
			//CW_motor();
			printf("moter cw\r\n");
		}
		else if(return_motor_start_flag&&return_motor_ANTICW_flag&&uper_motor_ANTICW_flag)
		{
			//ANTICW_motor();
			printf("motor_anticw\r\n");
			count=0;
			motorflag=0;
		}
	}
}


/**
*******************下面注释掉的代码是通过 位带 操作实现IO口控制**************************************
	
int main(void)
{ 
 
	delay_init(168);		  //初始化延时函数
	LED_Init();		        //初始化LED端口
  while(1)
	{
     LED0=0;			  //LED0亮
	   LED1=1;				//LED1灭
		 delay_ms(500);
		 LED0=1;				//LED0灭
		 LED1=0;				//LED1亮
		 delay_ms(500);
	 }
}
**************************************************************************************************
 **/	
	
/**
*******************下面注释掉的代码是通过 直接操作寄存器 方式实现IO口控制**************************************
int main(void)
{ 
 
	delay_init(168);		  //初始化延时函数
	LED_Init();		        //初始化LED端口
	while(1)
	{
     GPIOF->BSRRH=GPIO_Pin_9;//LED0亮
	   GPIOF->BSRRL=GPIO_Pin_10;//LED1灭
		 delay_ms(500);
     GPIOF->BSRRL=GPIO_Pin_9;//LED0灭
	   GPIOF->BSRRH=GPIO_Pin_10;//LED1亮
		 delay_ms(500);

	 }
 }	 
**************************************************************************************************
**/	
 



