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
#include "string.h"
#include <stdarg.h> // Required for variable arguments
#include <stdio.h>  // Required for vsnprintf

#define flash_start_address (0x08060000)

void read_sensor_status(I2C_HandleTypeDef hi2c1,uint8_t slave_address,uint16_t internal_address, UART_HandleTypeDef huart2)
{

	  // init empty reg to store status
	  uint8_t status_reg;

	  //read from WHO_AM_I register to check status of sensor
      HAL_I2C_Mem_Read(&hi2c1, slave_address,internal_address, I2C_MEMADD_SIZE_8BIT, &status_reg,1,100);
	  // check if sensor status returns correct value
	  if(status_reg== 0x68){
		  // Transmission success
			uart_print_debug(huart2, "I2C transmit okay\r\n");
			uart_print_debug(huart2, "WHO_AM_I = 0x%02X\r\n", status_reg);

	  }
	  else
	  {
		  uart_print_debug(huart2, "I2C Transmit error \r\n");

	  }

}


float read_temp_data(I2C_HandleTypeDef hi2c1,uint8_t slave_address,UART_HandleTypeDef huart2)
{

	// buffer to store data
	uint8_t temperature_data[2];

	// register of sensor to read temperature data
	uint8_t temp_data_reg = 0x41;

	int len;
	char tx_buffer[32];
	char temp_decimal_value[32];


	// read data from temperature register
	HAL_I2C_Mem_Read(&hi2c1, slave_address, temp_data_reg, I2C_MEMADD_SIZE_8BIT, temperature_data, 2, 100);


	int16_t temp_data_dec= (int16_t)(temperature_data[0] << 8 | temperature_data[1]);
	float temperature_celsius = ((float)temp_data_dec / 340.0f) + 36.53f;


	return temperature_celsius;

}


void MPU6050_Init(I2C_HandleTypeDef hi2c1, uint8_t slave_address)
{
    uint8_t check;
    uint8_t data;

    // 1. Check device ID WHO_AM_I (should be 0x68)
    HAL_I2C_Mem_Read(&hi2c1, slave_address, 0x75, 1, &check, 1, 100);
    if (check == 0x68)
    {
        // 2. Wake up sensor: write 0x00 to PWR_MGMT_1 (0x6B)
        data = 0;
        HAL_I2C_Mem_Write(&hi2c1, slave_address, 0x6B, 1, &data, 1, 100);

        // 3. Set accelerometer config ±2g (ACCEL_CONFIG, 0x1C)
        data = 0;
        HAL_I2C_Mem_Write(&hi2c1, slave_address, 0x1C, 1, &data, 1, 100);

        // 4. Set gyroscope config ±250 °/s (GYRO_CONFIG, 0x1B)
        data = 0;
        HAL_I2C_Mem_Write(&hi2c1, slave_address, 0x1B, 1, &data, 1, 100);
    }
}



float read_accel_x(I2C_HandleTypeDef hi2c1,uint8_t slave_address,UART_HandleTypeDef huart2)
{

	uint8_t accel_x_reg = 0x3B;

	// create 6 bytes data buffer to store all data
	uint8_t accel_x_raw[6];

	// read from specific register address and store in 8 bit buffer
	HAL_I2C_Mem_Read(&hi2c1, slave_address, accel_x_reg, I2C_MEMADD_SIZE_8BIT, accel_x_raw, 2, 100);

	// shift first 8 bit to the lleft and perform or operation for reading accel  x
	int16_t accel_x_data = (int16_t)(accel_x_raw[0] << 8 | accel_x_raw[1]);
	//uart_print_debug(huart2, "Accel  : %d \r \n",accel_x_data);


	// scale data
	float accel_x_g = accel_x_data / 16384.0f;

	return accel_x_g;

}


void write_data_to_flash(UART_HandleTypeDef huart2, uint16_t temp_data)
{
	// define status
	HAL_StatusTypeDef status;
	HAL_StatusTypeDef erase_status;

	HAL_StatusTypeDef writing_to_flash_status;
	HAL_StatusTypeDef lock_status;

	int len;
	static FLASH_EraseInitTypeDef EraseInitStruct;


	// for uart debugging
	char flash_unlock_error[] ="Unlocking flash success \r\n";
	char flash_erase_success[] = "Erase sector success \r\n";
	char write_data_flash_success[] = "Success writing data to flash \r\n";
	char flash_lock_success[]="Locking flash success \r\n";


	// unlock flash and check
	status=HAL_FLASH_Unlock();
	if(status == HAL_OK)
	{

	    uart_print_debug(huart2, "unlockign flash success \r\n");
	}

	// init erase struct and fill in parameters
	// erase only sectors
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;

	// eras only sector 7 as it is the lowest memory block for safety purpose
	EraseInitStruct.Sector = FLASH_SECTOR_7;

	// erase only 1 sector
	EraseInitStruct.NbSectors = 1;


	// set voltage range for current dev board 2.7v~3.6V
	EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;


	int sector_error=0;
	erase_status=HAL_FLASHEx_Erase(&EraseInitStruct, &sector_error);


	// checks if manage to eras data
	if(erase_status == HAL_OK)
	{
	    uart_print_debug(huart2, "erasing flash success \r\n");


		 // writing 16 bits temp data
		 writing_to_flash_status=HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, flash_start_address, temp_data);


		// checks if manage to write data
		if(writing_to_flash_status == HAL_OK)
		{

		    uart_print_debug(huart2, "writing data to flash success \r\n");

		}
		else
		{

			uint32_t flash_error = HAL_FLASH_GetError();
		    uart_print_debug(huart2, "FLASH ERROR :  \r\n", flash_error);

		}



	}

	// lock for safety
	lock_status = HAL_FLASH_Lock();
	if(lock_status == HAL_OK)
	{
	    uart_print_debug(huart2, "locking flash success \r\n");

	}



}

void read_data_from_flash(UART_HandleTypeDef huart2)
{

	uint16_t read_temp_data = *(uint16_t*)flash_start_address;
	uart_print_debug(huart2, "Temperature data : %u \r\n",read_temp_data);

}



void uart_print_debug(UART_HandleTypeDef huart2,const char *mssg,...)
{


	char tx_buffer[128]; // Increased buffer size for safety
	va_list args;
	int len;

	va_start(args, mssg);

	len = vsnprintf(tx_buffer, sizeof(tx_buffer), mssg, args);
	va_end(args);

	HAL_UART_Transmit(&huart2,(uint8_t*)tx_buffer,len,100);
}






