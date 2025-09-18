/*
 * mikro.h
 *
 *  Created on: Aug 25, 2025
 *      Author: alifn
 */

#ifndef MIKRO_H_
#define MIKRO_H_
#include "stm32f4xx_hal.h" // Include the necessary HAL library header
#include <stdbool.h>

extern uint16_t gpio_pins[10];

void digitalWrite_pinA(uint8_t pin, bool state);


#endif /* MIKRO_H_ */
