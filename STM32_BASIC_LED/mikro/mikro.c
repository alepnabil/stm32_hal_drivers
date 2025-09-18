/*
 * mikro.c
 *
 *  Created on: Aug 25, 2025
 *      Author: alifn
 */

#include "mikro.h"
#include <stdbool.h>
#include <stdint.h>

uint16_t gpio_pins[10]={

		GPIO_PIN_0,
		GPIO_PIN_1,
		GPIO_PIN_2,
		GPIO_PIN_3,
		GPIO_PIN_4,
		GPIO_PIN_5,
		GPIO_PIN_6,
		GPIO_PIN_7,
		GPIO_PIN_8,
		GPIO_PIN_9,


};



void digitalWrite_pinA(uint8_t pin, bool state)
{
	 HAL_GPIO_WritePin(GPIOA, gpio_pins[pin], state);

}
