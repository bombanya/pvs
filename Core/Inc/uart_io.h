/*
 * uart_io.h
 *
 *  Created on: 1 окт. 2022 г.
 *      Author: Merlin
 */

#ifndef INC_UART_IO_H_
#define INC_UART_IO_H_

#include "queue.h"
#include "main.h"

void uart_io_init(struct queue* input, struct queue* output);
void uart_io_write_from_buffer(uint8_t* buffer, size_t len);

#endif /* INC_UART_IO_H_ */
