#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "BJDJ.h"
#include "adc.h"
#include "key.h"
#include "exti.h"
#include "timer.h"
#include "usart3.h" 
#include "string.h"	 	 
#include "gps.h"
#include "usart4.h"
				  	 
u8 USART1_TX_BUF[USART3_MAX_RECV_LEN]; 					//??1,?????
nmea_msg gpsx; 											//GPS??
__align(4) u8 dtbuf[50];   								//?????

u8 uper_motor_CW_flag=1;
u8 uper_motor_ANTICW_flag=1;

int main(void)
{ 
	u16 i,rxlen;
	u16 lenx;
	u8 upload=0;
  u16 adcx;
	u8 key=0XFF;
	u8 count=0;
	u8 motorflag=0;
	extern u8 return_motor_start_flag;
	extern u8 return_motor_CW_flag;
	extern u8 return_motor_ANTICW_flag;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);    //初始化延时函数
	uart_init(115200);	//初始化串口波特率为115200
	usart3_init(38400);			//?????3????38400
	usart4_init(38400);
	LED_Init();					//初始化LED 
	Adc_Init();         //初始化ADC
	EXTIX_Init();       //初始化外部中断输入 
	BJDJ_Init();
  TIM3_Int_Init(10-1,8400-1);
	if(Ublox_Cfg_Rate(1000,1)!=0)	//???????????1000ms,????GPS??????. 
	{
		while((Ublox_Cfg_Rate(1000,1)!=0)&&key)	//????,???????NEO-6M,???????
		{
			usart3_init(9600);				//?????3????9600(EEPROM?????????,????9600.)
	  	Ublox_Cfg_Prt(38400);			//???????????38400
			usart3_init(38400);				//?????3????38400
			Ublox_Cfg_Tp(1000000,100000,1);	//??PPS?1????1?,?????100ms	    
			key=Ublox_Cfg_Cfg_Save();		//????  
		}	  					 
	}
	while(1)
	{
		if(USART3_RX_STA&0X8000)		//????????
		{
			rxlen=USART3_RX_STA&0X7FFF;	//??????
			for(i=0;i<rxlen;i++)USART1_TX_BUF[i]=USART3_RX_BUF[i];	   
 			USART3_RX_STA=0;		   	//???????
			USART1_TX_BUF[i]=0;			//???????
			GPS_Analysis(&gpsx,(u8*)USART1_TX_BUF);//?????
			if(upload)
				printf("\r\n%s\r\n",USART1_TX_BUF);//???????????1
 		}
		upload=1;

		if((lenx%500)==0)
			LED0=!LED0; 	    				 
		lenx++;	
		
		adcx=Get_Adc_Average(ADC_Channel_5,20);//获取通道5的转换值，20次取平均
		if(adcx>=3850)
		{
			count++;
			uper_motor_CW_flag=0;
			//printf("press high\r\n");
		}
		else
		{
			if(count>=10)
			{
				//printf("already\r\n");
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
		delay_ms(250);	
		//printf("%d %d %d\r\n",return_motor_start_flag,return_motor_CW_flag,uper_motor_CW_flag);
		if(return_motor_start_flag&&return_motor_CW_flag&&uper_motor_CW_flag)
		{
			//CW_motor();
			//printf("moter cw\r\n");
		}
		else if(return_motor_start_flag&&return_motor_ANTICW_flag&&uper_motor_ANTICW_flag)
		{
			//ANTICW_motor();
			//printf("motor_anticw\r\n");
			count=0;
			motorflag=0;
		}
	}
}




