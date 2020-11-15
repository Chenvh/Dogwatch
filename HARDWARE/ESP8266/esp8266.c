#include "esp8266.h"
#include "string.h"
#include "usart.h"
#include "usart4.h"
#include "stm32f4xx.h"
#include "sys.h" 
#include "delay.h"

//ESP8266模块和PC进入透传模式
void esp8266_start_trans(void)
{
	//设置工作模式 1：station模式   2：AP模式  3：兼容 AP+station模式
	while(esp8266_send_cmd("AT","OK",50));
	while(esp8266_send_cmd("AT+CWMODE=1","OK",50));
	
	//让Wifi模块重启的命令
	while(esp8266_send_cmd("AT+RST","ready",600));
	
	delay_ms(1000);         //延时3S等待重启成功
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	
	//让模块连接上自己的路由
	while(esp8266_send_cmd("AT+CWJAP=\"HUAWEI\",\"1269886119\"","WIFI GOT IP",600));
	
	//=0：单路连接模式     =1：多路连接模式
	while(esp8266_send_cmd("AT+CIPMUX=1","OK",20));
	
	//建立TCP连接  这四项分别代表了 要连接的ID号0~4   连接类型  远程服务器IP地址   远程服务器端口号
	//while(esp8266_send_cmd("AT+CIPSTART=\"TCP\",\"xxx.xxx.xxx.xxx\",xxxx","CONNECT",200));
	
	//是否开启透传模式  0：表示关闭 1：表示开启透传
	//esp8266_send_cmd("AT+CIPMODE=1","OK",200);
	
	//透传模式下 开始发送数据的指令 这个指令之后就可以直接发数据了
	//esp8266_send_cmd("AT+CIPSEND","OK",50);
	while(esp8266_send_cmd("AT+CIPSERVER=1,8899","OK",600));
	delay_ms(1000);         //延时3S等待重启成功
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	
	while(esp8266_send_cmd("AT+CIPSEND=0,12","OK",600));
	
	u3_printf("wifi_init_ok");
	
}

//ESP8266退出透传模式   返回值:0,退出成功;1,退出失败
//配置wifi模块，通过想wifi模块连续发送3个+（每个+号之间 超过10ms,这样认为是连续三次发送+）
u8 esp8266_quit_trans(void)
{
	u8 result=1;
	u3_printf("+++");
	delay_ms(1000);					//等待500ms太少 要1000ms才可以退出
	result=esp8266_send_cmd("AT","OK",20);//退出透传判断.
	if(result)
		printf("quit_trans failed!");
	else
		printf("quit_trans success!");
	return result;
}


u8 senddata_cmd(u16 len,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART4_RX_STA=0;
	u4_printf("AT+CIPSEND=0,%d\r\n",len);	//发送命令
	printf("AT+CIPSEND=0,%d\r\n",len);
	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_ms(10);
			if(USART4_RX_STA&0X8000)//接收到期待的应答结果
			{
				if(esp8266_check_cmd(ack))
				{
					printf("ack:%s\r\n",(u8*)ack);
					break;//得到有效数据 
				}
					USART4_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
} 



//向ESP8266发送命令
//cmd:发送的命令字符串;ack:期待的应答结果,如果为空,则表示不需要等待应答;waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果);1,发送失败
u8 esp8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART4_RX_STA=0;
	u4_printf("%s\r\n",cmd);	//发送命令
	printf("%s\r\n",cmd);
	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_ms(10);
			if(USART4_RX_STA&0X8000)//接收到期待的应答结果
			{
				if(esp8266_check_cmd(ack))
				{
					printf("ack:%s\r\n",(u8*)ack);
					break;//得到有效数据 
				}
					USART4_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
} 


//ESP8266发送命令后,检测接收到的应答
//str:期待的应答结果
//返回值:0,没有得到期待的应答结果;其他,期待应答结果的位置(str的位置)
u8* esp8266_check_cmd(u8 *str)
{
	char *strx=0;
	if(USART4_RX_STA&0X8000)		//接收到一次数据了
	{ 
		USART4_RX_BUF[USART4_RX_STA&0X7FFF]=0;//添加结束符
		strx=strstr((const char*)USART4_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}


//向ESP8266发送数据
//cmd:发送的命令字符串;waittime:等待时间(单位:10ms)
//返回值:发送数据后，服务器的返回验证码
u8* esp8266_send_data(u8 *cmd,u16 waittime)
{
	char temp[5];
	char *ack=temp;
	USART4_RX_STA=0;
	u4_printf("%s",cmd);	//发送命令
	if(waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_ms(10);
			if(USART4_RX_STA&0X8000)//接收到期待的应答结果
			{
				USART4_RX_BUF[USART4_RX_STA&0X7FFF]=0;//添加结束符
				ack=(char*)USART4_RX_BUF;
				printf("ack:%s\r\n",(u8*)ack);
				USART4_RX_STA=0;
				break;//得到有效数据 
			} 
		}
	}
	return (u8*)ack;
} 


