#ifndef __USART_H__
#define __USART_H__
#include <reg52.h>
#define uint32_t unsigned long
#define uint16_t unsigned int
#define int16_t  int
#define uint8_t unsigned char

//sbit led1=P0^0;
//sbit led2=P0^1;
//sbit led3=P0^2;
void Usart_Int(uint32_t BaudRatePrescaler);
void USART_send_byte(uint8_t Tx_data);
void send_3out(uint8_t *Data,uint8_t length,uint8_t send);
void send_out(uint16_t *Data,uint8_t length,uint8_t send);
void USART_Send(uint8_t *Buffer, uint8_t Length);
extern uint8_t send_ok,Receive_ok,Sonar_data[8];
extern 	uint16_t time_count;
#endif