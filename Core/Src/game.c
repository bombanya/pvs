/*
 * game.c
 *
 *  Created on: Oct 27, 2022
 *      Author: s-jar
 */

#include "game.h"

#include "main.h"

#define PLAY_TIME_MS 500
#define START_TIMEOUT_MS 3000
#define SPEED_NORM 10

static enum mode {
	SYNTH,
	RYTHM
} mode = SYNTH;

static bool lights = true;

static bool speaker = true;

static struct melody *melody;

static int i = 0;

static bool processing = false;

static bool pressed = false;

static uint32_t timeout = 0;

static uint32_t score = 0;

static uint32_t modifier = 0;

static enum speed {
	SLOW = 8,
	MEDIUM = 10,
	FAST = 12
} speed;

static void do_next_command();

static void play_sound(sound s);

static void game_play_melody();

static sound get_current_sound();

static void change_peripheral();

static void change_mode();

static void score_start();

static void score_pressed(bool right);

static void change_game_speed();

void game_register_melody(struct melody * m)
{
	melody = m;
}

void game_key_pressed(enum keys key){
	switch(key){
	case KEY_1: case KEY_2: case KEY_3:
	case KEY_4: case KEY_5: case KEY_6:
	case KEY_7: case KEY_8: case KEY_9:
		if (mode == SYNTH) {
			sound s = sound_by_key(key);
			play_sound(s);
		} else {
			sound cur = get_current_sound();
			if(cur.key == key && !pressed) {
				score_pressed(true);
				pressed = true;
			} else {
				score_pressed(false);
			}
		}
		break;
	case KEY_A:
		change_peripheral();
		break;
	case KEY_PLUS:
		change_game_speed();
		break;
	case KEY_ENTER:
		change_mode();
		break;
	case NO_KEY: default:
		break;
	}
}

void game_timeout_callback()
{
	if (processing){
		timeout --;
		if (timeout == 0) {
			if(!pressed)
				score_pressed(false);
			LED_turn_off_all();
			speaker_stop();
			processing = false;
			if(mode == RYTHM) {
				i++;
				do_next_command();
			}
		}
	}
}

bool game_is_in_process(){
	return mode == RYTHM;
}

uint32_t game_get_score(){
	return score;
}

static void game_play_melody()
{
	i = 0;
	timeout = START_TIMEOUT_MS;
	score_start();
	pressed = false;
	main_notify_game_started();
	processing = true;
}

static void do_next_command()
{
	if (i < melody->size) {
		pressed = false;
		union player_command command = melody->commands[i];
		if (command.command == PLAY) {
			sound s = command.play.s;
			play_sound(s);
		} else {
			timeout = command.wait.wait_ms * speed / SPEED_NORM;
		}
		processing = true;
	} else {
		mode = SYNTH;
		main_notify_game_finished();
	}
}

static void play_sound(sound s)
{
	if (speaker)
		speaker_play(s.note);
	if (lights)
		LED_set(s.color, s.brightness);
	timeout = PLAY_TIME_MS * speed / SPEED_NORM;
	processing = true;
}

static sound get_current_sound()
{
	union player_command command = melody->commands[i];
	if (command.command == PLAY) {
		return command.play.s;
	} else {
		return no_sound;
	}
}

static void change_peripheral()
{
	if (speaker && lights) {
		speaker = false;
	} else if(lights) {
		lights = false;
		speaker = true;
	} else {
		lights = true;
	}
}

static void change_mode()
{
	if (mode == SYNTH) {
		mode = RYTHM;
		game_play_melody();
	} else {
		mode = SYNTH;
		processing = false;
		i = 0;
	}
}

static void change_game_speed(){
	switch(speed){
	case FAST:
		speed = SLOW;
		break;
	case MEDIUM:
		speed = FAST;
		break;
	case SLOW:
		speed = MEDIUM;
		break;
	default:
		speed = MEDIUM;
		break;
	}
}

static void score_start()
{
	score = 0;
	modifier = 1;
}

static void score_pressed(bool right)
{
	if (right) {
		score += modifier;
		modifier++;
	} else {
		modifier = 1;
	}
}
