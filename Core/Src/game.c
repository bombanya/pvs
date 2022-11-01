/*
 * game.c
 *
 *  Created on: Oct 27, 2022
 *      Author: s-jar
 */

#include "game.h"

#include "main.h"

#define PLAY_TIME_MS 2000
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
	SLOW = 12,
	MEDIUM = 10,
	FAST = 5
} speed = MEDIUM;

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
	{
		sound s = sound_by_key(key);
		print_string(melody_sound_to_string(s));
		if (mode == SYNTH) {
			pressed = true;
			play_sound(s);
		} else {
			sound cur = get_current_sound();
			if (cur.key == key && !pressed) {
				score_pressed(true);
				pressed = true;
			} else {
				score_pressed(false);
			}
		}
		break;
	}
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
	i = -1;
	timeout = START_TIMEOUT_MS;
	score_start();
	pressed = true;
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
			pressed = true;
		}
		processing = true;
	} else {
		mode = SYNTH;
		main_notify_game_finished();
	}
}

static void play_sound(sound s)
{
	LED_turn_off_all();
	if (speaker)
		speaker_play(s.note);
	if (lights)
		LED_set(s.color, s.brightness);
	timeout = PLAY_TIME_MS * speed / SPEED_NORM;
	processing = true;
}

static sound get_current_sound()
{
	if (i == 27) {
		print_string("break\n\r");
	}
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
		print_string("only lights\n\r");
	} else if(lights) {
		lights = false;
		speaker = true;
		print_string("only speaker\n\r");
	} else {
		lights = true;
		print_string("speaker & lights\n\r");
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
		LED_turn_off_all();
		speaker_stop();
		main_notify_game_finished();
	}
}

static void change_game_speed(){
	switch(speed){
	case FAST:
		speed = SLOW;
		print_string("SLOW mode\n\r");
		break;
	case MEDIUM:
		speed = FAST;
		print_string("FAST mode\n\r");
		break;
	case SLOW:
		speed = MEDIUM;
		print_string("MEDIUM mode\n\r");
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
		print_string("correct!\n\r");
		score += modifier;
		modifier++;
	} else {
		print_string("miss!\n\r");
		modifier = 1;
	}
}
