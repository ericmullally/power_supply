/*
 * I2C.h
 *
 *  Created on: Feb 16, 2023
 *      Author: Eric
 */
#pragma once
#ifndef I2C_H_
#define I2C_H_

#include <stm32f401xc.h>


void i2c1_init(void);
void i2c_read_byte(char sadd, char memadd, char* data );
void i2c_burst_read(char sadd, char memadd, int n, char* data );
void i2c_burst_write(char sadd, char memadd, int n, char* data);
#endif /* I2C_H_ */
