#ifndef __EXTI_H__
#define __EXTI_H__
#include <reg52.h>

#ifndef		uint32_t
#define uint32_t unsigned long
#endif

#ifndef		uint16_t
#define uint16_t unsigned int
#endif

#ifndef		  uint8_t
#define uint8_t unsigned char
#endif
void Exti_Int(bit n,uint8_t Mode);
extern uint8_t BIT;
#endif