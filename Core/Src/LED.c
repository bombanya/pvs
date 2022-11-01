/*
 * LED.c
 *
 *  Created on: Sep 18, 2022
 *      Author: s-jar
 */
#include "LED.h"
#include "main.h"
#include "stm32f4xx_hal_gpio.h"
#include "tim.h"

#define LED_COUNTER_PERIOD 1000

static char* color_names[] = {"RED", "GREEN", "YELLOW"};

void LED_set(enum color col, enum brightness brightness)
{
	switch(col){
	case RED:
		htim4.Instance->CCR4 = LED_COUNTER_PERIOD * brightness / 100;
		break;
	case GREEN:
		htim4.Instance->CCR2 = LED_COUNTER_PERIOD * brightness / 100;
		break;
	case YELLOW:
		htim4.Instance->CCR3 = LED_COUNTER_PERIOD * brightness / 100;
		break;
	}
}

void LED_turn_off_all() {
	htim4.Instance->CCR4 = 0;
	htim4.Instance->CCR2 = 0;
	htim4.Instance->CCR3 = 0;
}

char* LED_color_name(enum color col) {
	return color_names[col];
}

char * LED_brightness_name(enum brightness brightness) {
	switch(brightness) {
	case(FULL): return "FULL";
	case(HALF): return "HALF";
	case(LOW): return "LOW";
	case(OFF): return "OFF";
	default: return "";
	}
}
