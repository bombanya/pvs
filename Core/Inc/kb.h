/*
 * kb.h
 *
 *  Created on: Oct 27, 2022
 *      Author: Merlin
 */

#ifndef INC_KB_H_
#define INC_KB_H_

#include "main.h"
#include "queue.h"

enum kb_mode {
	TEST,
	WORK
};

void kb_init(struct queue*, enum kb_mode);
bool kb_input_is_empty();
uint8_t kb_input_pop();
void kb_tim_callback();
void kb_change_mode();

#endif /* INC_KB_H_ */
