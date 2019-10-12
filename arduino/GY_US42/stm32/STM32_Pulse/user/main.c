#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "string.h"
 #include "LED.h"

static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_X;
  
  /* 4����ռ���ȼ���4����Ӧ���ȼ� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  /*��ռ���ȼ��ɴ���жϼ���͵��ж�*/
	/*��Ӧ���ȼ����ȼ�ִ��*/
	NVIC_X.NVIC_IRQChannel = USART2_IRQn;//�ж�����
  NVIC_X.NVIC_IRQChannelPreemptionPriority = 0;//��ռ���ȼ�
  NVIC_X.NVIC_IRQChannelSubPriority = 0;//��Ӧ���ȼ�
  NVIC_X.NVIC_IRQChannelCmd = ENABLE;//ʹ���ж���Ӧ
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
	bytes[2]=data;//�����ֽ�
	bytes[3]=bytes[0]+bytes[1]+bytes[2];
	USART2_send_byte(bytes[0]);//����֡ͷ�������ֽڡ�У���
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
u8  TIM2CH1_CAPTURE_STA=0;	//ͨ��1���벶��״̬		  �ø���λ�������־������λ���������  				
u16	TIM2CH1_CAPTURE_UPVAL;	//ͨ��1���벶��ֵ
u16	TIM2CH1_CAPTURE_DOWNVAL;	//ͨ��1���벶��ֵ
u32 tempup1=0;//�����ܸߵ�ƽ��ʱ��us��λ
u32 tim2_T;
 /*
 Keil: MDK5.10.0.2
MCU:stm32f103c8
Ӳ���ӷ���
GY_US42---STM32
1��GY_US42_DT---STM32_PA0,����ߵ�ƽʱ��
2��GY_US42_CR---STM32_PA1,�������壬����һ�ξ������
3��VCC-----------VCC
4��GND-----------GND
5��PS-----------GND


ע���жϺ���λ��stm32f10x_it.c
��ϵ��ʽ��
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
	delay_ms(1);//�ȴ�ģ���ʼ�����

	while(1)
	{

  
		Send_Plus();//���������ź�
   	TIM_Cmd(TIM2,ENABLE ); 	//ʹ�ܶ�ʱ��2
		delay_ms(100);
   	TIM_Cmd(TIM2,DISABLE ); 	//�رն�ʱ��2
  	if(TIM2CH1_CAPTURE_STA&0X80)
		{
		  distance=340*tempup1/20000;
		  data=distance;
	    send_out(&data,1,0x45);//�ϴ�����λ��
			TIM2CH1_CAPTURE_STA=0;
			
		}
		
	}
}
