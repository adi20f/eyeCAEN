#include "stc_it.h"
#include "usart.h"
#include "string.h"
#include "exti.h"

uint8_t stata=0;
//定时器0中断
void timer0() interrupt 1  //
 {
         TF0=0;
         TH0=0;
         TL0=0;
		 BIT=2;	
   }
    //外部中断0
void EXTI1_IRQHandler(void)interrupt 0
{     
   	 TR0=0;          //关闭定时器1
     EX0=0;          //关闭外部中断0
     time_count =TH0<<8|TL0;     
	 BIT=1;
	 IE0=0;
}
//串口中断
void USART_IRQHandler(void)interrupt 4
{
	static uint8_t i=0,rebuf[7]={0};
	uint8_t sum=0;
    if(TI)//发送完成标志
	{
	  TI=0;//清发送完成标志
	  send_ok=0;//缓存标志置0 
	}
	if(RI)//接收完成标志
	{
		rebuf[i++]=SBUF;
		RI=0;//清中断接收标志
		if (rebuf[0]!=0x5a)//帧头不对
			i=0;	
		if ((i==2)&&(rebuf[1]!=0x5a))//帧头不对
			i=0;
	
		if(i>3)//i等于4时，已经接收到数据量字节rebuf[3]
		{
			if(i!=(rebuf[3]+5))//判断是否接收一帧数据完毕
				return;	
			switch(rebuf[2])//接收完毕后处理
			{
				case 0x45:
					if(!Receive_ok)//当数据处理完成后才接收新的数据
					{
						memcpy(Sonar_data,rebuf,7);//拷贝接收到的数据
						Receive_ok=1;//接收完成标志
					}
					break;
				case 0x15:break;//原始数据接收，可模仿0x45的方式
				case 0x35:break;
			}
			i=0;//缓存清0
		}
	
	}

}
