/*
 * side_button.h
 *
 *  Created on: Sep 18, 2022
 *      Author: s-jar
 */

#ifndef INC_SIDE_BUTTON_H_
#define INC_SIDE_BUTTON_H_

#include <inttypes.h>
#include <stdbool.h>

enum press_type{
	SHORT = 0,
	LONG
};

bool side_button_get_pressed();

enum press_type side_button_get_pressed_type();

uint32_t side_button_get_time_last_pressed();

void side_button_pressed_callback();

void side_button_reset_time_last_pressed();

#endif /* INC_SIDE_BUTTON_H_ */
