#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "string.h"
 #include "LED.h"

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
	
	NVIC_X.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_X.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_X.NVIC_IRQChannelSubPriority = 0;
	NVIC_X.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_X);
}
void send_com(u8 data)
{
	u8 bytes[4]={0};
	bytes[0]=0xa5;
	bytes[1]=0xa5;
	bytes[2]=data;//功能字节
	bytes[3]=bytes[0]+bytes[1]+bytes[2];
	USART2_send_byte(bytes[0]);//发送帧头、功能字节、校验和
	USART2_send_byte(bytes[1]);
	USART2_send_byte(bytes[2]);
	USART2_send_byte(bytes[3]);
}
void Gpio_init(void)
{
	GPIO_InitTypeDef GPIO_A; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_A.GPIO_Pin = GPIO_Pin_1;//TRIG_PIN
	GPIO_A.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_A.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_A);
}
void Send_Plus(void)
{
	  GPIO_SetBits(GPIOA,GPIO_Pin_1);
	   delay_us(25);
   	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
}
void delay(uint16_t x)
{
  while(x--);
}
u8  TIM2CH1_CAPTURE_STA=0;	//通道1输入捕获状态		  用高两位做捕获标志，低六位做溢出计数  				
u16	TIM2CH1_CAPTURE_UPVAL;	//通道1输入捕获值
u16	TIM2CH1_CAPTURE_DOWNVAL;	//通道1输入捕获值
u32 tempup1=0;//捕获总高电平的时间us单位
u32 tim2_T;
 /*
 Keil: MDK5.10.0.2
MCU:stm32f103c8
硬件接法：
GY_US42---STM32
1、GY_US42_DT---STM32_PA0,捕获高电平时间
2、GY_US42_CR---STM32_PA1,发送脉冲，启动一次距离测量
3、VCC-----------VCC
4、GND-----------GND
5、PS-----------GND


注：中断函数位于stm32f10x_it.c
联系方式：
http://shop62474960.taobao.com/?spm=a230r.7195193.1997079397.2.9qa3Ky&v=1
*/
int main(void)
{
  u8 sum=0,i=0;
	int16_t data=0;
  uint16_t distance=0;
	float time=0;
	delay_init(72);
	Gpio_init();
	
	NVIC_Configuration();
	Usart_Int(115200);
	//Usart_Int2(9600);
	TIM2_Cap_Init(0xffff,71);
	delay_ms(1);//等待模块初始化完成

	while(1)
	{

  
		Send_Plus();//发送脉冲信号
   	TIM_Cmd(TIM2,ENABLE ); 	//使能定时器2
		delay_ms(100);
   	TIM_Cmd(TIM2,DISABLE ); 	//关闭定时器2
  	if(TIM2CH1_CAPTURE_STA&0X80)
		{
		  distance=340*tempup1/20000;
		  data=distance;
	    send_out(&data,1,0x45);//上传给上位机
			TIM2CH1_CAPTURE_STA=0;
			
		}
		
	}
}
