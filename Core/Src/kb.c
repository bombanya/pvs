/*
 * kb.c
 *
 *  Created on: Nov 1, 2022
 *      Author: Merlin
 */

#include "kb.h"
#include "pca9538.h"
#include "uart_io.h"
#include "main.h"
#include <stdio.h>
#include <string.h>

#define KB_ADDR 0xE2
#define PRESS_MIN_TICKS 50

static struct queue* input_queue;
static enum kb_mode cur_mode;

static uint8_t rows[4] = {0xFE, 0xFD, 0xFB, 0xF7};
static uint8_t btns[3] = {0x10, 0x20, 0x40};

static char output_buffer[50];
static uint8_t row_data;

static uint8_t cur_row = 0;
static uint32_t btns_press_time[12] = {0};
static bool input_block = false;
static uint8_t total_btns_pressed = 0;
static int8_t input = -1;

void kb_init(struct queue* queue, enum kb_mode mode) {
	input_queue = queue;
	cur_mode = mode;

	uint8_t buf = 0;
	pca9538_write_register(KB_ADDR, POLARITY_INVERSION, &buf);
	pca9538_write_register(KB_ADDR, OUTPUT_PORT, &buf);
}

bool kb_input_is_empty() {
	return queue_is_empty(input_queue);
}

uint8_t kb_input_pop() {
	return queue_pop(input_queue);
}

void kb_tim_callback() {
	pca9538_write_register_it(KB_ADDR, CONFIG, &rows[cur_row]);
}

void kb_change_mode() {
	if (cur_mode == TEST) {
		cur_mode = WORK;
		print_string("keyboard is in WORK mode\n\r");
	}
	else {
		cur_mode = TEST;
		print_string("keyboard is in TEST mode\n\r");
	}
}

int8_t kb_check_row(uint8_t row_n) {
	uint8_t tmp = 0;
	pca9538_write_register(KB_ADDR, OUTPUT_PORT, &tmp);
	pca9538_write_register(KB_ADDR, CONFIG, &(rows[row_n]));
	pca9538_read_inputs(KB_ADDR, &row_data);

	for (uint8_t i = 0; i < 3; i++) {
		if (!(row_data & btns[i])) {
			return i;
		}
	}
	return -1;
}
static void handle_input() {
	if (cur_mode == TEST) {
		sprintf(output_buffer, "%c\n\r", input);
		uart_io_write_from_buffer((uint8_t*) output_buffer, strlen(output_buffer));
	}
	else queue_push(input_queue, input);
	input = -1;
}

void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c) {
	pca9538_read_inputs_it(KB_ADDR, &row_data);
}

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c) {
	row_data &= 0x70;

	uint8_t btn;
	for (uint8_t i = 0; i < 3; i++) {
		btn = cur_row * 3 + i;
		if (!(row_data & btns[i])) {
			total_btns_pressed++;
			if (btns_press_time[btn] == 0) btns_press_time[btn] = HAL_GetTick();
			else if (HAL_GetTick() - btns_press_time[btn] > PRESS_MIN_TICKS
					&& !input_block)
				input = btn;
		}
		else btns_press_time[btn] = 0;
	}

	if (cur_row == 3) {
		if (total_btns_pressed > 1) {
			input_block = true;
		}
		else if (total_btns_pressed == 1 && !input_block) {
			if (input != -1) {
				input_block = true;
				handle_input();
			}
		}
		else if (total_btns_pressed == 0) input_block = false;

		cur_row = 0;
		total_btns_pressed = 0;
	}
	else {
		cur_row++;
		pca9538_write_register_it(KB_ADDR, CONFIG, &rows[cur_row]);
	}
}
