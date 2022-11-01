/*
 * uart_io.c
 *
 *  Created on: 1 окт. 2022 г.
 *      Author: Merlin
 */

#include "uart_io.h"
#include "usart.h"


static struct queue* input_queue;
static struct queue* output_queue;

static uint8_t input_byte;
static uint8_t output_byte;

static uint8_t nonblocking_write_in_progress = 0;

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

void uart_io_init(struct queue* input, struct queue* output) {
	input_queue = input;
	output_queue = output;
	HAL_UART_Receive_IT(&huart6, &input_byte, 1);
}

void uart_io_write_from_buffer(uint8_t *buffer, size_t len) {
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


