/*
 * LED.c
 *
 *  Created on: Sep 18, 2022
 *      Author: s-jar
 */
#include "LED.h"
#include "main.h"
#include "stm32f4xx_hal_gpio.h"

void LED_turn_on(enum color col)
{
	switch(col){
	case RED:
		HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_SET);
		break;
	case GREEN:
		HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_SET);
		break;
	case YELLOW:
		HAL_GPIO_WritePin(YELLOW_LED_GPIO_Port, YELLOW_LED_Pin, GPIO_PIN_SET);
		break;
	}
}

void LED_turn_off(enum color col)
{
	switch(col){
	case RED:
		HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_RESET);
		break;
	case GREEN:
		HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_RESET);
		break;
	case YELLOW:
		HAL_GPIO_WritePin(YELLOW_LED_GPIO_Port, YELLOW_LED_Pin, GPIO_PIN_RESET);
		break;
	}
}

void LED_blink(enum color col, uint32_t period)
{
	uint32_t dark = period / 2;
	uint32_t light = period - dark;
	LED_turn_on(col);
	HAL_Delay(light);
	LED_turn_off(col);
	HAL_Delay(dark);
}
