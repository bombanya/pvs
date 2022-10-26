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

enum brightness {
	FULL = 100,
	HALF = 50,
	LOW = 20,
	OFF = 0
};

void LED_set(enum color col, enum brightness);

void LED_turn_off_all();

#endif /* INC_LED_H_ */
