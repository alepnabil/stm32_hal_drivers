/*
 * mpu6050.c
 *
 *  Created on: Oct 3, 2025
 *      Author: alifn
 */



#include <stdint.h>
#include "stdio.h"
#include "main.h"
#include "mpu6050.h"

void read_sensor_status(I2C_HandleTypeDef hi2c1,uint8_t slave_address,uint16_t internal_address, UART_HandleTypeDef huart2)
{

	// init empty reg to store status
	 uint8_t status_reg;
	 // empty status output pritn
	 char tx_buffer[32];

	 // len of message
	 int len;
	  //read from WHO_AM_I register to check status of sensor
      HAL_I2C_Mem_Read(&hi2c1, slave_address,internal_address, I2C_MEMADD_SIZE_8BIT, &status_reg,1,100);
	  // check if sensor status returns correct value
	  if(status_reg== 0x68){
		  // Transmission success
			char error_msg[] = "I2C transmit okay\r\n";
			// transmit message
			HAL_UART_Transmit(&huart2, (uint8_t*)error_msg, sizeof(error_msg)-1, 100);
			// output register value
			len = snprintf(tx_buffer, sizeof(tx_buffer), "WHO_AM_I = 0x%02X\r\n", status_reg);
			HAL_UART_Transmit(&huart2, (uint8_t*)tx_buffer, len, 100);


	  }
	  else
	  {
		  // Transmission failed
		  char ok_msg[] = "I2C Transmit not ok\r\n";
		  HAL_UART_Transmit(&huart2, (uint8_t*)ok_msg, sizeof(ok_msg)-1, 100);
	  }

}
