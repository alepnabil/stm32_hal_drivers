/*
 * 74hc595.h
 *
 *  Created on: Sep 19, 2025
 *      Author: alifn
 */

#ifndef _74HC595_H_
#define _74HC595_H_



#endif /* 74HC595_H_ */


void init_74hc595(GPIO_TypeDef* GPIO_PORT, uint16_t OE_PIN, uint16_t RCLCK, uint16_t SHCLR);
void en_register_clock_output(void);
void en_enable_output(void);
void en_clear_clock(void);



