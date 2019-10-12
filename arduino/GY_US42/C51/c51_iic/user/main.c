#include <reg52.h>
#include "usart.h"
#include "iic.h"  
/*
Ӳ���ӷ���
GY-US42V1----C51
SCL---P3^6
SDA---P3^7
C51---FT232
TX ---RX
RX ---TX
���˵����

ע��
	�жϺ���λ��stc_it.c
��ϵ��ʽ��
http://shop62474960.taobao.com/?spm=a230r.7195193.1997079397.2.9qa3Ky&v=1
*/

void delay(unsigned int x)
{
	while(x--);
}
int main(void)
{

	Usart_Int(9600);
 	while(1)
	{
	   	uint16_t diatance=0;
	    requestRange((0xe1),&diatance);		 //��ȡ��������
		takeRangeReading(0xe0);				 //���Ͳ�������

		send_out(&diatance,1,0x45);		 //������λ��
		delay(60000);	
	}
}
