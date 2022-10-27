/*
 * melody.h
 *
 *  Created on: Oct 27, 2022
 *      Author: s-jar
 */

#ifndef INC_MELODY_H_
#define INC_MELODY_H_

#include <stddef.h>
#include "LED.h"
#include "speaker.h"

struct sound {
	enum notes note;
	enum color color;
	enum brightness brightness;
	char symbol;
};

typedef struct sound sound;

extern const sound ut;

extern const sound re;

extern const sound mi;

extern const sound fa;

extern const sound sol;

extern const sound la;

extern const sound si;

extern const sound ut_2;

extern const sound re_2;

enum command {
	WAIT, PLAY
};

struct play_command {
	enum command command;
	sound s;
};

struct wait_command {
	enum command command;
	uint32_t wait_ms;
};

union player_command {
	enum command command;
	struct play_command play;
	struct wait_command wait;
};

struct melody {
	size_t size;
	size_t current;
	union player_command *commands;
};

struct melody melody_init(size_t sound_cnt);

void melody_add_play(struct melody *melody, sound s);

void melody_add_wait(struct melody *melody, uint32_t wait_ms);

void melody_destroy(struct melody);

#endif /* INC_MELODY_H_ */
