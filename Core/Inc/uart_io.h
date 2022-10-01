/*
 * uart_io.h
 *
 *  Created on: 1 окт. 2022 г.
 *      Author: Merlin
 */

#ifndef INC_UART_IO_H_
#define INC_UART_IO_H_

#include "queue.h"

enum uart_io_mode {
	BLOCKING,
	NONBLOCKING
};

void uart_io_init(struct queue* input, struct queue* output, enum uart_io_mode mode);
void uart_io_read_data();
void uart_io_write_from_buffer(uint8_t* buffer, size_t len);
void uart_io_change_mode();

#endif /* INC_UART_IO_H_ */
