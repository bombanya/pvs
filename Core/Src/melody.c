/*
 * melody.c
 *
 *  Created on: Oct 27, 2022
 *      Author: s-jar
 */

#include <stdlib.h>
#include "melody.h"

const sound ut =
		{ .note = C, .color = GREEN, .brightness = FULL, .symbol = '1' };

const sound re =
		{ .note = D, .color = GREEN, .brightness = HALF, .symbol = '2' };

const sound mi = { .note = E, .color = GREEN, .brightness = LOW, .symbol = '3' };

const sound fa = { .note = F, .color = RED, .brightness = FULL, .symbol = '4' };

const sound sol = { .note = G, .color = RED, .brightness = HALF, .symbol = '5' };

const sound la = { .note = A, .color = RED, .brightness = LOW, .symbol = '6' };

const sound si =
		{ .note = B, .color = YELLOW, .brightness = FULL, .symbol = '7' };

const sound ut_2 = { .note = C_2, .color = YELLOW, .brightness = HALF, .symbol =
		'8' };

const sound re_2 = { .note = D_2, .color = YELLOW, .brightness = LOW, .symbol =
		'9' };

struct melody melody_init(size_t sound_cnt){
	struct melody m = {};
	m.size = sound_cnt;
	m.current = 0;
	m.commands = malloc(sound_cnt);
	return m;
}

void melody_destroy(struct melody melody){
	free(melody.commands);
	melody.size = 0;
	melody.current = 0;
}

void melody_add_play(struct melody *melody, sound s){
	if(melody->current < melody->size - 1){
		melody->current += 1;
		union player_command command = melody->commands[melody->current];
		command.command = PLAY;
		command.play.s = s;
	}
}

void melody_add_wait(struct melody *melody, uint32_t wait_ms){
	if(melody->current < melody->size -1){
		melody->current +=1;
		union player_command command = melody->commands[melody->current];
		command.command = WAIT;
		command.wait.wait_ms = wait_ms;
	}
}
