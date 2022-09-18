/*
 * LED.h
 *
 *  Created on: Sep 18, 2022
 *      Author: s-jar
 */

#ifndef INC_LED_H_
#define INC_LED_H_

#include <inttypes.h>

enum color {
	RED,
	GREEN,
	YELLOW
};

void LED_turn_on(enum color col);

void LED_turn_off(enum color col);

void LED_blink(enum color col, uint32_t period);

#endif /* INC_LED_H_ */
