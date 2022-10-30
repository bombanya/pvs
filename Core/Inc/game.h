/*
 * game.h
 *
 *  Created on: Oct 27, 2022
 *      Author: s-jar
 */

#ifndef INC_GAME_H_
#define INC_GAME_H_

#include <stdbool.h>
#include "melody.h"

void game_register_melody(struct melody *);

void game_key_pressed(enum keys);

void game_timeout_callback();

bool game_is_in_process();

uint32_t game_get_score();

#endif /* INC_GAME_H_ */
