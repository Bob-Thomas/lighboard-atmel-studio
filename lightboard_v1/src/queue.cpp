#include "queue.h"

Queue::Queue() : count(0) {
	head = 0;
	tail = QUEUE_SIZE - 1;
}

void Queue::enqueue(uint8_t data) {
	tail = (tail +1) % QUEUE_SIZE;
	a[tail] = data;
	if (count < QUEUE_SIZE - 1) {
		count++;
	}
}

uint8_t Queue::dequeue() {
	uint8_t val = a[head];
	head = (head + 1) % QUEUE_SIZE;
	if (count > 0) {
		count--;
	}
	return val;
}

uint8_t Queue::get_count() {
	return count;
}