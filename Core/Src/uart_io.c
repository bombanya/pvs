/*
 * uart_io.c
 *
 *  Created on: 1 окт. 2022 г.
 *      Author: Merlin
 */

#include "uart_io.h"
#include "usart.h"
#include <stdio.h>

struct queue* input_queue;
struct queue* output_queue;
enum uart_io_mode cur_mode;

uint8_t input_byte;
uint8_t output_byte;

uint8_t nonblocking_write_in_progress = 0;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart == &huart6) {
		queue_push(input_queue, input_byte);
		HAL_UART_Receive_IT(&huart6, &input_byte, 1);
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart == &huart6) {
		if (!queue_is_empty(output_queue)) {
			output_byte = queue_pop(output_queue);
			HAL_UART_Transmit_IT(&huart6, &output_byte, 1);
		}
		else {
			nonblocking_write_in_progress = 0;
		}
	}
}

void uart_io_init(struct queue* input, struct queue* output, enum uart_io_mode mode) {
	input_queue = input;
	output_queue = output;
	cur_mode = mode;

	if (cur_mode == NONBLOCKING) HAL_UART_Receive_IT(&huart6, &input_byte, 1);
}

void uart_io_read_data() {
	if (cur_mode == BLOCKING) {
		HAL_StatusTypeDef result = HAL_UART_Receive(&huart6, &input_byte, 1, 5);
		if (result == HAL_OK) queue_push(input_queue, input_byte);
	}
}

void uart_io_write_from_buffer(uint8_t* buffer, size_t len) {
	if (cur_mode == BLOCKING) {
		HAL_UART_Transmit(&huart6, buffer, len, 100);
	}
	else {
		HAL_NVIC_DisableIRQ(USART6_IRQn);
		for (size_t i = 0; i < len; i++) {
			queue_push(output_queue, buffer[i]);
		}
		if (!nonblocking_write_in_progress) {
			nonblocking_write_in_progress = 1;
			output_byte = queue_pop(output_queue);
			HAL_UART_Transmit_IT(&huart6, &output_byte, 1);
		}
		HAL_NVIC_EnableIRQ(USART6_IRQn);
	}
}

void uart_io_change_mode() {
	if (cur_mode == BLOCKING) {
		cur_mode = NONBLOCKING;
		HAL_UART_Receive_IT(&huart6, &input_byte, 1);
		HAL_NVIC_EnableIRQ(USART6_IRQn);
		printf("\nnonblocking mode\n");
	}
	else {
		HAL_UART_AbortReceive(&huart6);
		while (nonblocking_write_in_progress) {}
		HAL_NVIC_DisableIRQ(USART6_IRQn);
		cur_mode = BLOCKING;
		printf("\nblocking mode\n");
	}
}


