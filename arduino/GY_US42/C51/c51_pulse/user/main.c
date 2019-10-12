#include <reg52.h>
#include "usart.h" 
#include "exti.h"

/*
Ӳ���ӷ���

C51����Ϊ11.0592Mhz
�������� = 12*ʱ������=12*��1/11.0592��us=1.085us
����=T_count*1.085*340/2

GY_US42----------c51
1��PS------------GND 
2��GND-----------GND
3��VCC-----------5V
4��GY_US42_DT---c51_P3^2,�жϲ���͵�ƽ
5��c51_TX--------FT232
6��GY_US42_CR----c51_P3^6����������
���˵��:
�ó�����ô��ڷ�ʽ��ȡģ��������ݣ�������9600


ע���жϺ���λ��stc_it.c
��ϵ��ʽ��
http://shop62474960.taobao.com/?spm=a230r.7195193.1997079397.2.9qa3Ky&v=1

*/
sbit Trig = P3^6;
sbit Echo =P3^2 ;

void send_com(uint8_t datas)
{
   uint8_t bytes[4]={0};
	bytes[0]=0xa5;
	bytes[1]=0xa5;
	bytes[2]=datas;//�����ֽ�
	USART_Send(bytes,4);//����֡ͷ�������ֽڡ�У���
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
    TMOD |= 0x01;//��ʱ��0��������ʽ1��16λ����ֵ
	ET0=1;
	delay(1000);

 	while(1)
	{
	
	   	TH0=0;          //��ʱ��1����
        TL0=0;          //��ʱ��1����
        TF0=0;          //���������־
	   ////////��������/////////////
	    Trig=1;
		delay_20us();
	    Trig=0;
		BIT=0;
	   ////////�ȴ�Echo����///////////////
		while(!Echo) ;
		 TR0=1;          //������ʱ��1
		 EX0=1;          //���ⲿ�ж�0
	   ///////�ȴ�Echo����///////////////
	   // while(BIT==0) ;
	    delay(10000);	  
		if(BIT==1)
		{
	   	  temp=((float)time_count)/2000000*340;
		   distance=temp*100;
		  send_out(&distance,1,0x45);//�ϴ�����λ�� 
	       
		}else
			USART_send_byte(0xff);
		BIT=0;
		
	}
}
