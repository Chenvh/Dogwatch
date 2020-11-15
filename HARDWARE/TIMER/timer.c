#include "timer.h"
#include "BJDJ.h"
#include "led.h"

extern u8 return_motor_start_flag;
extern u8 return_motor_CW_flag;
extern u8 return_motor_ANTICW_flag;
extern u8 uper_motor_CW_flag;
extern u8 uper_motor_ANTICW_flag;


extern vu16 USART3_RX_STA;
extern vu16 USART4_RX_STA;

void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///ʹ��TIM3ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

 
void TIM7_Int_Init(u16 arr,u16 psc)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);//TIM7????    
	
	//???TIM7???
	TIM_TimeBaseStructure.TIM_Period = arr; //???????????????????????????	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //??????TIMx???????????
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //??????:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM??????
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //??????????TIMx???????
 
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE ); //?????TIM7??,??????
	
	TIM_Cmd(TIM7,ENABLE);//?????7
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//?????0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//????2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ????
	NVIC_Init(&NVIC_InitStructure);	//??????????VIC???
	
}
void TIM5_Int_Init(u16 arr,u16 psc)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);//TIM7????    
	
	//???TIM7???
	TIM_TimeBaseStructure.TIM_Period = arr; //???????????????????????????	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //??????TIMx???????????
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //??????:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM??????
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //??????????TIMx???????
 
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); //?????TIM7??,??????
	
	TIM_Cmd(TIM5,ENABLE);//?????7
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//?????0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//????2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ????
	NVIC_Init(&NVIC_InitStructure);	//??????????VIC???
	
}

void TIM5_IRQHandler(void)
{ 	
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)//?????
	{	 			   
		USART4_RX_STA|=1<<15;	//??????
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update  );  //??TIM7??????    
		TIM_Cmd(TIM5, DISABLE);  //??TIM7 
	}	    
}


void TIM7_IRQHandler(void)
{ 	
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)//?????
	{	 			   
		USART3_RX_STA|=1<<15;	//??????
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update  );  //??TIM7??????    
		TIM_Cmd(TIM7, DISABLE);  //??TIM7 
	}	    
}


//��ʱ��3�жϷ�����
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //����ж�
	{
		LED0=!LED0;	
		if(return_motor_start_flag&&return_motor_CW_flag&&uper_motor_CW_flag)
		{
			CW_motor();		
		}
		else if(return_motor_start_flag&&return_motor_ANTICW_flag&&uper_motor_ANTICW_flag)
		{
			ANTICW_motor();
		}		
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
}
