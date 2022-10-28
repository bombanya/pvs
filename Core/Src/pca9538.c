/*
 * pca9538.c
 *
 *  Created on: Oct 27, 2022
 *      Author: Merlin
 */

#include "pca9538.h"
#include "i2c.h"

void pca9538_write_register(uint16_t addr, enum pca9538_reg reg, uint8_t* buf) {
	HAL_I2C_Mem_Write(&hi2c1, addr | 1, reg, 1, buf, 1, 100);
}

void pca9538_write_register_it(uint16_t addr, enum pca9538_reg reg, uint8_t* buf) {
	HAL_I2C_Mem_Write_IT(&hi2c1, addr | 1, reg, 1, buf, 1);
}

void pca9538_read_inputs_it(uint16_t addr, uint8_t* buf) {
	HAL_I2C_Mem_Read_IT(&hi2c1, addr & 0xFFE, INPUT_PORT, 1, buf, 1);
}
