/*
 * delay.h
 *
 *  Created on: Aug 18, 2024
 *      Author: ayhan
 */

#ifndef INC_DELAY_H_
#define INC_DELAY_H_


#include "stm32f407xx.h"


void delay_us(uint16_t us);
void timer7_init(void);
void timer6_init(void);
uint32_t gettick(void);


#endif /* INC_DELAY_H_ */
