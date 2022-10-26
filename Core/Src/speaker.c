/*
 * speaker.c
 *
 *  Created on: Oct 26, 2022
 *      Author: Merlin
 */

#include "speaker.h"
#include "tim.h"

static TIM_OC_InitTypeDef pwm_config = {
		.OCMode = TIM_OCMODE_PWM1,
		.OCPolarity = TIM_OCPOLARITY_HIGH,
		.OCNPolarity = TIM_OCNPOLARITY_HIGH,
		.OCFastMode = TIM_OCFAST_DISABLE,
		.OCIdleState = TIM_OCIDLESTATE_RESET,
		.OCNIdleState = TIM_OCNIDLESTATE_RESET
};

void speaker_play(struct note note) {
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	htim1.Init.Period =  1 / note.frequency * 1000000;
	HAL_TIM_Base_Init(&htim1);
	pwm_config.Pulse = htim1.Init.Period / 2;
	HAL_TIM_PWM_ConfigChannel(&htim1, &pwm_config, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
}

void speaker_stop() {
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
}
