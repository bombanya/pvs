/*
 * side_button.c
 *
 *  Created on: Sep 18, 2022
 *      Author: s-jar
 */

#include "side_button.h"

#include "main.h"
#include "stm32f4xx_hal_gpio.h"

#define SHORT_PRESS_MIN_TICKS 50
#define LONG_PRESS_MIN_TICKS 1000

static enum press_type pressed_type;

static bool pressed;

static uint32_t time_last_pressed;

enum press_type side_button_get_pressed_type()
{
	return pressed_type;
}

bool side_button_get_pressed()
{
	bool res = pressed;
	pressed = false;
	return res;
}

uint32_t side_button_get_time_last_pressed()
{
	return time_last_pressed;
}

void side_button_pressed_callback()
{
	static uint32_t pressed_cnt = 0;
	if(HAL_GPIO_ReadPin(SIDE_BUTTON_GPIO_Port, SIDE_BUTTON_Pin))
				pressed_cnt = HAL_GetTick();
			else{
				uint32_t tick = HAL_GetTick();
				pressed_cnt = tick - pressed_cnt;
				if(pressed_cnt >= LONG_PRESS_MIN_TICKS){
					time_last_pressed = tick;
					pressed = true;
					pressed_type = LONG;
				}
				else if(pressed_cnt >= SHORT_PRESS_MIN_TICKS){
					time_last_pressed = tick;
					pressed = true;
					pressed_type = SHORT;
				}
			}
}
