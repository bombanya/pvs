/*
 * game.h
 *
 *  Created on: Oct 27, 2022
 *      Author: s-jar
 */

#ifndef INC_GAME_H_
#define INC_GAME_H_

#include "melody.h"

void game_register_melody(struct melody *);

void game_play_melody();

void game_register_char(char);

#endif /* INC_GAME_H_ */
