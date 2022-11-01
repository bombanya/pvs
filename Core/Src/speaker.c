/*
 * speaker.c
 *
 *  Created on: Oct 26, 2022
 *      Author: Merlin
 */

#include "speaker.h"
#include "tim.h"

void speaker_play(enum notes note) {
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	htim1.Instance->ARR = 1.0 / note * 1000000;
	htim1.Instance->CCR1 = htim1.Instance->ARR / 2;
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
}

void speaker_stop() {
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
}
