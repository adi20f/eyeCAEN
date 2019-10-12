#ifndef __USART_H
#define __USART_H
#include "stm32f10x.h"

void Usart_Int(uint32_t BaudRatePrescaler);
void USART1_send_byte(uint8_t byte);
void USART2_send_byte(uint8_t byte);
void send_3out(uint8_t *data,uint8_t length,uint8_t send);
void send_out(int16_t *data,uint8_t length,uint8_t send);
void display(int16_t *num,u8 send,u8 count);
void CHeck(uint8_t *re_data);
void USART_Send(uint8_t *Buffer, uint8_t Length);
void USART_Send_bytes(uint8_t *Buffer, uint8_t Length);
void Usart_Int2(uint32_t BaudRatePrescaler);
void USART2_send_byte(uint8_t byte);
extern u8 Sonar_data[8],Receive_ok;
extern u8   TIM2CH1_CAPTURE_STA;	//ͨ��1���벶��״̬		  �ø���λ�������־������λ���������  				
extern u16	TIM2CH1_CAPTURE_UPVAL;	//ͨ��1���벶��ֵ
extern u16	TIM2CH1_CAPTURE_DOWNVAL;	//ͨ��1���벶��ֵ
extern u32 tempup1;//�����ܸߵ�ƽ��ʱ��us��λ
extern u32 tim2_T;

#endif
