/*
 * game.c
 *
 *  Created on: Oct 27, 2022
 *      Author: s-jar
 */

#include "game.h"

static struct melody *melody;

static int i = 0;

static void do_next_command(){};

void game_register_melody(struct melody * m){
	melody = m;
}

void game_play_melody(){
	i = 0;
	do_next_command();
}

void game_register_char(char);
