/*
 * 74hc.h
 *
 *  Created on: Sep 15, 2025
 *      Author: alifn
 */

#ifndef 74HC_H_
#define 74HC_H_



#endif /* 74HC_H_ */



// initialize function
void init_74hc(GPIO_TypeDef* GPIO_PORT, uint8_t SER, uint8_t RCLCK, uint8_t SRCLCK, uint8_t SRCLR,
			uint8_t OE);


void en_shift_register_clck(void);
void en_register_clock_output(void);
void en_clear_clock(void);
void en_enable_output(void);
void en_enable_output(void);
