#include "stc_it.h"
#include "usart.h"
#include "string.h"
#include "exti.h"

uint8_t stata=0;
//��ʱ��0�ж�
void timer0() interrupt 1  //
 {
         TF0=0;
         TH0=0;
         TL0=0;
		 BIT=2;	
   }
    //�ⲿ�ж�0
void EXTI1_IRQHandler(void)interrupt 0
{     
   	 TR0=0;          //�رն�ʱ��1
     EX0=0;          //�ر��ⲿ�ж�0
     time_count =TH0<<8|TL0;     
	 BIT=1;
	 IE0=0;
}
//�����ж�
void USART_IRQHandler(void)interrupt 4
{
	static uint8_t i=0,rebuf[7]={0};
	uint8_t sum=0;
    if(TI)//������ɱ�־
	{
	  TI=0;//�巢����ɱ�־
	  send_ok=0;//�����־��0 
	}
	if(RI)//������ɱ�־
	{
		rebuf[i++]=SBUF;
		RI=0;//���жϽ��ձ�־
		if (rebuf[0]!=0x5a)//֡ͷ����
			i=0;	
		if ((i==2)&&(rebuf[1]!=0x5a))//֡ͷ����
			i=0;
	
		if(i>3)//i����4ʱ���Ѿ����յ��������ֽ�rebuf[3]
		{
			if(i!=(rebuf[3]+5))//�ж��Ƿ����һ֡�������
				return;	
			switch(rebuf[2])//������Ϻ���
			{
				case 0x45:
					if(!Receive_ok)//�����ݴ�����ɺ�Ž����µ�����
					{
						memcpy(Sonar_data,rebuf,7);//�������յ�������
						Receive_ok=1;//������ɱ�־
					}
					break;
				case 0x15:break;//ԭʼ���ݽ��գ���ģ��0x45�ķ�ʽ
				case 0x35:break;
			}
			i=0;//������0
		}
	
	}

}
