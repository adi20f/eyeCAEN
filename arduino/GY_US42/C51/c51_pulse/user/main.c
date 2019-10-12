#include <reg52.h>
#include "usart.h" 
#include "exti.h"

/*
硬件接法：

C51晶振为11.0592Mhz
机器周期 = 12*时钟周期=12*（1/11.0592）us=1.085us
距离=T_count*1.085*340/2

GY_US42----------c51
1、PS------------GND 
2、GND-----------GND
3、VCC-----------5V
4、GY_US42_DT---c51_P3^2,中断捕获低电平
5、c51_TX--------FT232
6、GY_US42_CR----c51_P3^6，发送脉冲
软件说明:
该程序采用串口方式获取模块距离数据，波特率9600


注：中断函数位于stc_it.c
联系方式：
http://shop62474960.taobao.com/?spm=a230r.7195193.1997079397.2.9qa3Ky&v=1

*/
sbit Trig = P3^6;
sbit Echo =P3^2 ;

void send_com(uint8_t datas)
{
   uint8_t bytes[4]={0};
	bytes[0]=0xa5;
	bytes[1]=0xa5;
	bytes[2]=datas;//功能字节
	USART_Send(bytes,4);//发送帧头、功能字节、校验和
}
void delay(uint16_t z)
{
 while(z--);
 
}
void delay_20us(void)
{ 
    unsigned char a ;
    for(a=0;a<100;a++);
}

uint16_t time_count=0;
int main(void)
{

    uint8_t sum=0,i=0;
	uint16_t distance=0;
	float temp=0;
	IP=0X01;
	Trig=0;
	Usart_Int(9600);
	Exti_Int(0,0);
    TMOD |= 0x01;//定时器0，工作方式1，16位计数值
	ET0=1;
	delay(1000);

 	while(1)
	{
	
	   	TH0=0;          //定时器1清零
        TL0=0;          //定时器1清零
        TF0=0;          //计数溢出标志
	   ////////发送脉冲/////////////
	    Trig=1;
		delay_20us();
	    Trig=0;
		BIT=0;
	   ////////等待Echo拉高///////////////
		while(!Echo) ;
		 TR0=1;          //启动定时器1
		 EX0=1;          //打开外部中断0
	   ///////等待Echo拉低///////////////
	   // while(BIT==0) ;
	    delay(10000);	  
		if(BIT==1)
		{
	   	  temp=((float)time_count)/2000000*340;
		   distance=temp*100;
		  send_out(&distance,1,0x45);//上传给上位机 
	       
		}else
			USART_send_byte(0xff);
		BIT=0;
		
	}
}
