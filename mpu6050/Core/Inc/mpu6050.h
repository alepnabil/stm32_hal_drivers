/*
 * mpu6050.h
 *
 *  Created on: Oct 3, 2025
 *      Author: alifn
 */

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_


#endif /* INC_MPU6050_H_ */
extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart2;


void read_sensor_status(I2C_HandleTypeDef hi2c1,uint8_t slave_address,uint16_t internal_address,UART_HandleTypeDef huart2);

