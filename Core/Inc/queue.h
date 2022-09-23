/*
 * queue.h
 *
 *  Created on: Sep 23, 2022
 *      Author: Merlin
 */

#ifndef INC_QUEUE_H_
#define INC_QUEUE_H_

#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>

struct queue {
	uint8_t *arr;
	size_t len;
	size_t head;
	size_t tail;
};

struct queue queue_init(size_t len);
void queue_destroy(struct queue*);
uint8_t queue_pop(struct queue*);
void queue_push(struct queue*, uint8_t elem);
bool queue_is_empty(struct queue*);
bool queue_is_full(struct queue*);

#endif /* INC_QUEUE_H_ */
