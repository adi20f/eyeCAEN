#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "string.h"
 #include "LED.h"
 /*
 Keil: MDK5.10.0.2
MCU:stm32f103c8
硬件接法：
GY_US42---STM32
1、GY_US42_RX---STM32_TX2,STM32发送A5 56 01 FC 给模块
2、GY_US42_TX---STM32_RX2,模块发送距离数据给STM32
3、STM32_TX1---FT232,STM32将数据上传给上位机
4、PS---VCC,  PS高电平选择串口模式
软件说明:
该程序采用串口方式获取模块距离数据，USART1波特率115200，USART1波特率9600
注：中断函数位于stm32f10x_it.c
联系方式：
http://shop62474960.taobao.com/?spm=a230r.7195193.1997079397.2.9qa3Ky&v=1
*/
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_X;
  
  /* 4个抢占优先级，4个响应优先级 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  /*抢占优先级可打断中断级别低的中断*/
	/*响应优先级按等级执行*/
	NVIC_X.NVIC_IRQChannel = USART2_IRQn;//中断向量
  NVIC_X.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级
  NVIC_X.NVIC_IRQChannelSubPriority = 0;//响应优先级
  NVIC_X.NVIC_IRQChannelCmd = ENABLE;//使能中断响应
  NVIC_Init(&NVIC_X);
}
void send_com(u8 data)
{
	u8 bytes[4]={0};
	bytes[0]=0xa5;
	bytes[1]=0x56;
	bytes[2]=data;//功能字节
	bytes[3]=bytes[0]+bytes[1]+bytes[2];
	USART2_send_byte(bytes[0]);//发送帧头、功能字节、校验和
	USART2_send_byte(bytes[1]);
	USART2_send_byte(bytes[2]);
	USART2_send_byte(bytes[3]);
}

int main(void)
{
  u8 sum=0,i=0;
	int16_t data=0;
  uint16_t distance=0;
	delay_init(72);
	NVIC_Configuration();
	Usart_Int(115200);
	Usart_Int2(9600);
	
	delay_ms(1);//等待模块初始化完成

	while(1)
	{
		send_com(0x01);//发送读指令
		delay_ms(100);
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
