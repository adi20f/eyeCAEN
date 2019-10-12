#include "stm32f10x.h"
#include "LED.h"
#include "IIC.h"
#include "delay.h"
#include "usart.h"
/*
Keil: MDK5.10.0.2
MCU:stm32f103c8
Ӳ���ӷ���
GY-US42V1---STM32
SCL---PB6
SDA---PB7
STM32 USART1---FT232
TX---RX
RX---TX
���˵��:

ע��
	�жϺ���λ��stm32f10x_it.c
��ϵ��ʽ��
http://shop62474960.taobao.com/?spm=a230r.7195193.1997079397.2.9qa3Ky&v=1
*/
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_X;
  
  /* 4����ռ���ȼ���4����Ӧ���ȼ� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  /*��ռ���ȼ��ɴ���жϼ���͵��ж�*/
	/*��Ӧ���ȼ����ȼ�ִ��*/
	NVIC_X.NVIC_IRQChannel = USART1_IRQn;//�ж�����
  NVIC_X.NVIC_IRQChannelPreemptionPriority = 0;//��ռ���ȼ�
  NVIC_X.NVIC_IRQChannelSubPriority = 0;//��Ӧ���ȼ�
  NVIC_X.NVIC_IRQChannelCmd = ENABLE;//ʹ���ж���Ӧ
  NVIC_Init(&NVIC_X);
}

int main(void)
{
	uint16_t diatance=0;
  u8 i=0;
	delay_init(72);
	LED_Int(GPIOB,GPIO_Pin_9,RCC_APB2Periph_GPIOB);
	NVIC_Configuration();
	Usart_Int(115200);
	I2C_GPIO_Config();
	
	 delay_ms(1);//�ȴ�ģ���ʼ�����
	// changeAddress(0xe0,0x40);//����IIC��ַ
	while(1)
	{

		requestRange(0XE1,&diatance);
		takeRangeReading(0XE0);

		send_out(&diatance,1,0x45);
		delay_ms(100);
		
	}
}
