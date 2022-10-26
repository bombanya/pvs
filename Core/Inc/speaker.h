/*
 * speaker.h
 *
 *  Created on: Oct 26, 2022
 *      Author: Merlin
 */

#ifndef SRC_SPEAKER_H_
#define SRC_SPEAKER_H_

#include "LED.h"

struct note {
	uint16_t frequency;
	enum color color;
	enum brightness brightness;
};

void speaker_play(struct note);

void speaker_stop();

#endif /* SRC_SPEAKER_H_ */
