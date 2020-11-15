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

//ALIENTEK ̽����STM32F407������ ʵ��1
//�����ʵ�� -�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com  
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK
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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);    //��ʼ����ʱ����
	uart_init(115200);	//��ʼ�����ڲ�����Ϊ115200
	LED_Init();					//��ʼ��LED 
 	//LCD_Init();         //��ʼ��LCD�ӿ�
	Adc_Init();         //��ʼ��ADC
	//BEEP_Init();        //��ʼ���������˿�
	EXTIX_Init();       //��ʼ���ⲿ�ж����� 
	/*POINT_COLOR=RED; 
	LCD_ShowString(30,50,200,16,16,"Explorer STM32F4");	
	LCD_ShowString(30,70,200,16,16,"ADC TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2014/5/6");	  
	POINT_COLOR=BLUE;//��������Ϊ��ɫ
	LCD_ShowString(30,130,200,16,16,"ADC1_CH5_VAL:");	      
	LCD_ShowString(30,150,200,16,16,"ADC1_CH5_VOL:0.000V");	//���ڹ̶�λ����ʾС����    */ 
	BJDJ_Init();
  TIM3_Int_Init(10-1,8400-1);
	
	while(1)
	{
		adcx=Get_Adc_Average(ADC_Channel_5,20);//��ȡͨ��5��ת��ֵ��20��ȡƽ��
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
		//LCD_ShowxNum(134,130,adcx,4,16,0);    //��ʾADCC�������ԭʼֵ
		//temp=(float)adcx*(3.3/4096);          //��ȡ�����Ĵ�С����ʵ�ʵ�ѹֵ������3.1111
		//adcx=temp;                            //��ֵ�������ָ�adcx��������ΪadcxΪu16����
		//LCD_ShowxNum(134,150,adcx,1,16,0);    //��ʾ��ѹֵ���������֣�3.1111�Ļ������������ʾ3
		//temp-=adcx;                           //���Ѿ���ʾ����������ȥ��������С�����֣�����3.1111-3=0.1111
		//temp*=1000;                           //С�����ֳ���1000�����磺0.1111��ת��Ϊ111.1���൱�ڱ�����λС����
		//LCD_ShowxNum(150,150,temp,3,16,0X80); //��ʾС�����֣�ǰ��ת��Ϊ��������ʾ����������ʾ�ľ���111.
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
*******************����ע�͵��Ĵ�����ͨ�� λ�� ����ʵ��IO�ڿ���**************************************
	
int main(void)
{ 
 
	delay_init(168);		  //��ʼ����ʱ����
	LED_Init();		        //��ʼ��LED�˿�
  while(1)
	{
     LED0=0;			  //LED0��
	   LED1=1;				//LED1��
		 delay_ms(500);
		 LED0=1;				//LED0��
		 LED1=0;				//LED1��
		 delay_ms(500);
	 }
}
**************************************************************************************************
 **/	
	
/**
*******************����ע�͵��Ĵ�����ͨ�� ֱ�Ӳ����Ĵ��� ��ʽʵ��IO�ڿ���**************************************
int main(void)
{ 
 
	delay_init(168);		  //��ʼ����ʱ����
	LED_Init();		        //��ʼ��LED�˿�
	while(1)
	{
     GPIOF->BSRRH=GPIO_Pin_9;//LED0��
	   GPIOF->BSRRL=GPIO_Pin_10;//LED1��
		 delay_ms(500);
     GPIOF->BSRRL=GPIO_Pin_9;//LED0��
	   GPIOF->BSRRH=GPIO_Pin_10;//LED1��
		 delay_ms(500);

	 }
 }	 
**************************************************************************************************
**/	
 



