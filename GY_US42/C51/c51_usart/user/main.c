#include <reg52.h>
#include "usart.h"
#include "iic.h"  
/*
Ӳ���ӷ���
GY_US42---c51
1��GY_US42_RX---c51_TX,c51��λ������A5 56 02 FD ��ģ���Զ����ָ��)��ģ��
2��c51_TX---FT232,STM32�������ϴ�����λ��
3��GY_US42_TX---c51_RX������ģ���������
4��PS-----VCC��pc�Ӹߵ�ƽѡ�񴮿�ģʽ
���˵��:
ģ��Ĭ��Ϊ9600��
�ó�����ô��ڷ�ʽ��ȡģ��������ݣ�������9600
���Ե�����λ���Ƚ�ģ�鴮�����ó�9600��Ȼ���ٽ������ϲ�����
ָ��:A5 58 AE AB,ģ���踴λ��Ч

ע���жϺ���λ��stc_it.c
��ϵ��ʽ��
http://shop62474960.taobao.com/?spm=a230r.7195193.1997079397.2.9qa3Ky&v=1
*/
void send_com(u8 datas)
{
   u8 bytes[4]={0};
	bytes[0]=0xa5;
	bytes[1]=0x56;
	bytes[2]=datas;//�����ֽ�
	USART_Send(bytes,4);//����֡ͷ�������ֽڡ�У���
}
void delay(uint16_t  x)
{
	while(x--);
}
int main(void)
{

    u8 sum=0,i=0;
	  uint16_t distance=0;
	Usart_Int(9600);
	delay(60000);
	send_com(0x02);//���Ͷ�rgbָ��
 	while(1)
	{
	
		delay(100);
		if(Receive_ok)//���ڽ������
		{
			for(sum=0,i=0;i<(Sonar_data[3]+4);i++)//rgb_data[3]=3
			sum+=Sonar_data[i];
			if(sum==Sonar_data[i])//У����ж�
			{
		       distance=(Sonar_data[4]<<8)|Sonar_data[5];
				send_out(&distance,1,0x45);//�ϴ�����λ��
			}
			Receive_ok=0;//����������ϱ�־
		}
	}
}
