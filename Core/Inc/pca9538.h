/*
 * pca9538.h
 *
 *  Created on: Nov 1, 2022
 *      Author: Merlin
 */

#ifndef INC_PCA9538_H_
#define INC_PCA9538_H_

#include <stdint.h>

enum pca9538_reg {
	INPUT_PORT = 0x00,
	OUTPUT_PORT = 0x01,
	POLARITY_INVERSION = 0x02,
	CONFIG = 0x03
};

void pca9538_write_register(uint16_t addr, enum pca9538_reg reg, uint8_t* buf);
void pca9538_write_register_it(uint16_t addr, enum pca9538_reg reg, uint8_t* buf);
void pca9538_read_inputs_it(uint16_t addr, uint8_t* buf);
void pca9538_read_inputs(uint16_t addr, uint8_t* buf);

#endif /* INC_PCA9538_H_ */
