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
			uart_print_debug(&huart2, "I2C transmit okay\r\n");
			uart_print_debug(&huart2, "WHO_AM_I = 0x%02X\r\n", status_reg);

	  }
	  else
	  {
		  uart_print_debug(&huart2, "I2C Transmit error \r\n");

	  }

}


float read_temp_data(I2C_HandleTypeDef hi2c1,uint8_t slave_address,UART_HandleTypeDef huart2)
{

	// buffer to store data
	uint8_t temperature_data[2];

	// register of sensor to read temperature data
	uint8_t temp_data_reg = 0x41;



	// read data from temperature register
	HAL_I2C_Mem_Read(&hi2c1, slave_address, temp_data_reg, I2C_MEMADD_SIZE_8BIT, temperature_data, 2, 100);


	int16_t temp_data_dec= (int16_t)(temperature_data[0] << 8 | temperature_data[1]);
	float temperature_celsius = ((float)temp_data_dec / 340.0f) + 36.53f;


	return temperature_celsius;

}


void MPU6050_Init(I2C_HandleTypeDef *hi2c1, uint8_t slave_address)
{
    uint8_t check;
    uint8_t data;

    // 1. Check device ID WHO_AM_I (should be 0x68)
    HAL_I2C_Mem_Read(hi2c1, slave_address, 0x75, 1, &check, 1, 100);
    if (check == 0x68)
    {
        // 2. Wake up sensor: write 0x00 to PWR_MGMT_1 (0x6B)
        data = 0;
        HAL_I2C_Mem_Write(hi2c1, slave_address, 0x6B, 1, &data, 1, 100);

        // 3. Set accelerometer config ±2g (ACCEL_CONFIG, 0x1C)
        data = 0;
        HAL_I2C_Mem_Write(hi2c1, slave_address, 0x1C, 1, &data, 1, 100);

        // 4. Set gyroscope config ±250 °/s (GYRO_CONFIG, 0x1B)
        data = 0;
        HAL_I2C_Mem_Write(hi2c1, slave_address, 0x1B, 1, &data, 1, 100);
    }
}



void read_accel_data(I2C_HandleTypeDef hi2c1,uint8_t slave_address,UART_HandleTypeDef huart2)
{

	uint8_t accel_x_reg = 0x3B;

	// create 6 bytes data buffer to store all data
	uint8_t accel_x_raw[6];

	// read from specific register address and store in 8 bit buffer
	// read 6 bytes of data to get all data
	HAL_I2C_Mem_Read(&hi2c1, slave_address, accel_x_reg, I2C_MEMADD_SIZE_8BIT, accel_x_raw, 6, 100);

	// shift first 8 bit to the lleft and perform or operation for reading accel  x
	int16_t accel_x_data = (int16_t)(accel_x_raw[0] << 8 | accel_x_raw[1]);
	int16_t accel_y_data = (int16_t)(accel_x_raw[2] << 8 | accel_x_raw[3]);
	int16_t accel_z_data = (int16_t)(accel_x_raw[4] << 8 | accel_x_raw[5]);



	//uart_print_debug(huart2, "Accel  : %d \r \n",accel_x_data);


	// scale data
	float accel_x_g = accel_x_data / 16384.0f;
	float accel_y_g = accel_y_data / 16384.0f;
	float accel_z_g = accel_z_data / 16384.0f;

	uart_print_debug(&huart2, "Accel X : %.3f \r \n",accel_x_g);
	uart_print_debug(&huart2, "Accel Y : %.3f \r \n",accel_y_g);
	uart_print_debug(&huart2, "Accel Z : %.3f \r \n",accel_z_g);

}

void read_gyro_data(I2C_HandleTypeDef hi2c1,uint8_t slave_address,UART_HandleTypeDef huart2)
{

	uint8_t accel_x_reg = 0x43;

	// create 6 bytes data buffer to store all data
	uint8_t gyro_raw_data[6];

	// read from specific register address and store in 8 bit buffer
	// read 6 bytes of data to get all data
	HAL_I2C_Mem_Read(&hi2c1, slave_address, accel_x_reg, I2C_MEMADD_SIZE_8BIT, gyro_raw_data, 6, 100);

	// shift first 8 bit to the lleft and perform or operation for reading accel  x
	int16_t gyro_x_data = (int16_t)(gyro_raw_data[0] << 8 | gyro_raw_data[1]);
	int16_t gyro_y_data = (int16_t)(gyro_raw_data[2] << 8 | gyro_raw_data[3]);
	int16_t gyro_z_data = (int16_t)(gyro_raw_data[4] << 8 | gyro_raw_data[5]);



	// scale data
	float gyro_x_g = gyro_x_data / 16384.0f;
	float gyro_y_g = gyro_y_data / 16384.0f;
	float gyro_z_g = gyro_z_data / 16384.0f;

	uart_print_debug(&huart2, "Gyro X : %.3f \r \n",gyro_x_g);
	uart_print_debug(&huart2, "Gyro Y : %.3f \r \n",gyro_y_g);
	uart_print_debug(&huart2, "Gyro Z : %.3f \r \n",gyro_z_g);

}


HAL_StatusTypeDef read_accel_data_interrupt(I2C_HandleTypeDef *hi2c1,uint8_t slave_address,UART_HandleTypeDef huart2, uint8_t *raw_data_arr)
{


	uint8_t accel_x_reg = 0x3B;

	//read_data_status=HAL_I2C_Master_Receive_IT(&hi2c1, slave_address, raw_data_arr, 6);
	return HAL_I2C_Mem_Read_IT(hi2c1, slave_address, accel_x_reg, I2C_MEMADD_SIZE_8BIT, raw_data_arr, 6);


}







void write_data_to_flash(UART_HandleTypeDef huart2, uint16_t temp_data)
{
	// define status
	HAL_StatusTypeDef status;
	HAL_StatusTypeDef erase_status;

	HAL_StatusTypeDef writing_to_flash_status;
	HAL_StatusTypeDef lock_status;

	static FLASH_EraseInitTypeDef EraseInitStruct;

	// unlock flash and check
	status=HAL_FLASH_Unlock();
	if(status == HAL_OK)
	{

	    uart_print_debug(&huart2, "unlockign flash success \r\n");
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
	    uart_print_debug(&huart2, "erasing flash success \r\n");


		 // writing 16 bits temp data
		 writing_to_flash_status=HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, flash_start_address, temp_data);


		// checks if manage to write data
		if(writing_to_flash_status == HAL_OK)
		{

		    uart_print_debug(&huart2, "writing data to flash success \r\n");

		}
		else
		{

			uint32_t flash_error = HAL_FLASH_GetError();
		    uart_print_debug(&huart2, "FLASH ERROR :  \r\n", flash_error);

		}

	}
	// lock for safety
	lock_status = HAL_FLASH_Lock();
	if(lock_status == HAL_OK)
	{
	    uart_print_debug(&huart2, "locking flash success \r\n");
	}

}

void read_data_from_flash(UART_HandleTypeDef huart2)
{

	uint16_t read_temp_data = *(uint16_t*)flash_start_address;
	uart_print_debug(&huart2, "Temperature data : %u \r\n",read_temp_data);

}



void uart_print_debug(UART_HandleTypeDef *huart2,const char *mssg,...)
{


	char tx_buffer[128]; // Increased buffer size for safety
	va_list args;
	int len;

	va_start(args, mssg);

	len = vsnprintf(tx_buffer, sizeof(tx_buffer), mssg, args);
	va_end(args);

	HAL_UART_Transmit(huart2,(uint8_t*)tx_buffer,len,100);
}






