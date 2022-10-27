/*
 * speaker.h
 *
 *  Created on: Oct 26, 2022
 *      Author: Merlin
 */

#ifndef SRC_SPEAKER_H_
#define SRC_SPEAKER_H_

/* Frequency in Hz */
enum notes {
	C = 262,
	D = 294,
	E = 330,
	F = 349,
	G = 392,
	A = 440,
	B = 494,
	C_2 = 554,
	D_2 = 622
};

void speaker_play(enum notes);

void speaker_stop();

#endif /* SRC_SPEAKER_H_ */
