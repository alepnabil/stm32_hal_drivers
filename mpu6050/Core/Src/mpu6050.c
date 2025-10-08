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

#define flash_start_address (0x0806 0000)

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


void read_temp_data(I2C_HandleTypeDef hi2c1,uint8_t slave_address,UART_HandleTypeDef huart2)
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

	// print out value
//	len = snprintf(tx_buffer, sizeof(tx_buffer), "temp_reg = 0x%02X\r\n", temperature_data);
//	HAL_UART_Transmit(&huart2, (uint8_t*)tx_buffer, len, 100);

	int16_t temp_data_dec= (int16_t)(temperature_data[0] << 8 | temperature_data[1]);
	float temperature_celsius = ((float)temp_data_dec / 340.0f) + 36.53f;

	//int len_temp = snprintf(temp_decimal_value,sizeof(temp_decimal_value), "Temp: %.2f C (Raw: %d)\r\n", temperature_celsius,raw_temp_value);
	//HAL_UART_Transmit(&huart2, (uint8_t*)temp_decimal_value, len_temp, 100);


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



float read_accel_x(I2C_HandleTypeDef hi2c1,uint8_t slave_address,UART_HandleTypeDef huart2)
{
	char accel_x_val[32];

	uint8_t accel_x_reg = 0x3B;
	uint8_t accel_x_raw[2];

	HAL_I2C_Mem_Read(&hi2c1, slave_address, accel_x_reg, I2C_MEMADD_SIZE_8BIT, accel_x_raw, 2, 100);

	int16_t accel_x_data = (int16_t)(accel_x_raw[0] << 8 | accel_x_raw[1]);

	float accel_x_g = accel_x_data / 16384.0f;

	return accel_x_g;

}


void write_data_to_flash(UART_HandleTypeDef huart2)
{
	HAL_StatusTypeDef status;
	HAL_StatusTypeDef erase_status;

	int len;
	static FLASH_EraseInitTypeDef EraseInitStruct;


	char flash_unlock_error[] ="Unlocking flash success\t";
	char flash_erase_success[] = "Erase sector success\t";

	status=HAL_FLASH_Unlock();
	if(status == HAL_OK)
	{
	    len = strlen(flash_unlock_error);
	    HAL_UART_Transmit(&huart2, (uint8_t*)flash_unlock_error, len, 100);
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

	if(erase_status == HAL_OK)
	{
		 len = strlen(flash_erase_success);
		 HAL_UART_Transmit(&huart2, (uint8_t*)flash_erase_success, len, 100);
	}







}














//void uart_print_reg_val(uint8_t *reg_val_ptr,UART_HandleTypeDef huart2)
//{
//		char data_buffer[32];
//
//
//	    len = snprintf(data_buffer, sizeof(data_buffer), "temp_reg = 0x%02X\r\n", &reg_val_ptr);
//		HAL_UART_Transmit(&huart2, (uint8_t*)tx_buffer, len, 100);
//
//}

