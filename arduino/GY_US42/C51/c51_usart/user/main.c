#include <reg52.h>
#include "usart.h"
#include "iic.h"  
/*
硬件接法：
GY_US42---c51
1、GY_US42_RX---c51_TX,c51复位将发送A5 56 02 FD （模块自动输出指令)给模块
2、c51_TX---FT232,STM32将数据上传给上位机
3、GY_US42_TX---c51_RX，接收模块距离数据
4、PS-----VCC，pc接高电平选择串口模式
软件说明:
模块默认为9600；
该程序采用串口方式获取模块距离数据，波特率9600
所以得用上位机先将模块串口设置成9600，然后再进行以上操作：
指令:A5 58 AE AB,模块需复位生效

注：中断函数位于stc_it.c
联系方式：
http://shop62474960.taobao.com/?spm=a230r.7195193.1997079397.2.9qa3Ky&v=1
*/
void send_com(u8 datas)
{
   u8 bytes[4]={0};
	bytes[0]=0xa5;
	bytes[1]=0x56;
	bytes[2]=datas;//功能字节
	USART_Send(bytes,4);//发送帧头、功能字节、校验和
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
	send_com(0x02);//发送读rgb指令
 	while(1)
	{
	
		delay(100);
		if(Receive_ok)//串口接收完毕
		{
			for(sum=0,i=0;i<(Sonar_data[3]+4);i++)//rgb_data[3]=3
			sum+=Sonar_data[i];
			if(sum==Sonar_data[i])//校验和判断
			{
		       distance=(Sonar_data[4]<<8)|Sonar_data[5];
				send_out(&distance,1,0x45);//上传给上位机
			}
			Receive_ok=0;//处理数据完毕标志
		}
	}
}
