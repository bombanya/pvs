/*
 * melody.c
 *
 *  Created on: Oct 27, 2022
 *      Author: s-jar
 */

#include <stdlib.h>
#include "melody.h"

const sound ut =
		{ .note = C, .color = GREEN, .brightness = FULL, .key = KEY_1};

const sound re =
		{ .note = D, .color = GREEN, .brightness = HALF, .key = KEY_2 };

const sound mi = { .note = E, .color = GREEN, .brightness = LOW, .key = KEY_3 };

const sound fa = { .note = F, .color = RED, .brightness = FULL, .key = KEY_4 };

const sound sol = { .note = G, .color = RED, .brightness = HALF, .key = KEY_5 };

const sound la = { .note = A, .color = RED, .brightness = LOW, .key = KEY_6 };

const sound si =
		{ .note = B, .color = YELLOW, .brightness = FULL, .key = KEY_7 };

const sound ut_2 = { .note = C_2, .color = YELLOW, .brightness = HALF, .key = KEY_8 };

const sound re_2 = { .note = D_2, .color = YELLOW, .brightness = LOW, .key = KEY_9 };

const sound no_sound = { .note = -1, .color = -1, .brightness = -1, .key = -1};

const sound sounds[] = {ut, re, mi, fa, sol, la, si, ut_2, re_2};

struct melody melody_init(size_t sound_cnt){
	struct melody m = {};
	m.size = sound_cnt;
	m.current = 0;
	m.commands = malloc(sound_cnt * sizeof(union player_command));
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
		union player_command *command = &(melody->commands[melody->current]);
		command->command = PLAY;
		command->play.s = s;
	}
}

void melody_add_wait(struct melody *melody, uint32_t wait_ms){
	if(melody->current < melody->size -1){
		melody->current +=1;
		union player_command *command = &(melody->commands[melody->current]);
		command->command = WAIT;
		command->wait.wait_ms = wait_ms;
	}
}

sound sound_by_key(enum keys key){
	size_t len = sizeof(sounds) / sizeof(sounds[0]);
	size_t i;
	for(i = 0; i < len; i++){
		if(sounds[i].key == key)
			return sounds[i];
	}
	return no_sound;
}
