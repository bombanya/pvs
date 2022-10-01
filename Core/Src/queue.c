/*
 * queue.c
 *
 *  Created on: Sep 23, 2022
 *      Author: Merlin
 */

#include "queue.h"
#include <malloc.h>

struct queue queue_init(size_t len) {
	struct queue q = {0};
	q.arr = malloc(len + 1);
	q.len = len + 1;
	return q;
}

void queue_destroy(struct queue* q) {
	free(q->arr);
}

uint8_t queue_pop(struct queue *q) {
	uint8_t elem = q->arr[q->head];
	q->head = (q->head + 1) % q->len;
	return elem;
}

void queue_push(struct queue *q, uint8_t elem){
	q->arr[q->tail] = elem;
	q->tail = (q->tail + 1) % q->len;
}

bool queue_is_empty(struct queue *q){
	return q->head == q->tail;
}

bool queue_is_full(struct queue *q){
	return (q->tail + 1) % q->len == q->head;
}
