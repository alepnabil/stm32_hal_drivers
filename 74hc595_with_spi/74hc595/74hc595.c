/*
 * 74hc595.c
 *
 *  Created on: Sep 19, 2025
 *      Author: alifn
 */

#include <stdint.h>
#include <stdio.h>
#include "stm32f4xx_hal.h"   // brings in HAL + GPIO_TypeDef
#include "stm32f4xx_hal_gpio.h"

#include "stm32f4xx_hal.h"

GPIO_TypeDef  *IC_PORT;
uint16_t IC_PIN[3];


// initialize the pins needed with 74hc595 ic
void init_74hc595(GPIO_TypeDef* GPIO_PORT, uint16_t OE_PIN, uint16_t RCLCK, uint16_t SHCLR)
{

	IC_PORT = GPIO_PORT;
	IC_PIN[0] = OE_PIN;
	IC_PIN[1] = RCLCK;
	IC_PIN[2] = SHCLR;

}


// latch data from the shift register to the output register
void en_register_clock_output(void)
{
	HAL_GPIO_WritePin(IC_PORT,IC_PIN[1],GPIO_PIN_RESET);
	HAL_GPIO_WritePin(IC_PORT,IC_PIN[1],GPIO_PIN_SET);
}


// clear clock to remove any bits in the registers
// used after initializing the ic
void en_clear_clock(void)
{
	HAL_GPIO_WritePin(IC_PORT, IC_PIN[2],GPIO_PIN_SET);


}

// master line for IC
void en_enable_output(void)
{
	HAL_GPIO_WritePin(IC_PORT, IC_PIN[0],GPIO_PIN_RESET);

}

