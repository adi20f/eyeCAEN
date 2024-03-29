#ifndef __DELAY_H
#define __DELAY_H
#include "stm32f10x.h"
void delay_init(uint8_t SYSCLK);
void delay_us(uint32_t nus);
void delay_ms(uint16_t nms);
void Time_Begin(void);
void Get_Time(float *time);
void TIM2_Cap_Init(u16 arr,u16 psc);
#endif

