/** 
 *  @class Queue
 *  
 *  @brief Simple queue for uint8_t items
 *
 *  @section DESCRIPTION
 *  
 * This is a very simple queue system for queuing uint8_t's in.
 * The maximum the queue is able to hold is 255 items.
 * After it reaches 255 items it will start overwriting the first received item
 *
**/


#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <sys\_stdint.h>

#define QUEUE_SIZE 255


class Queue
{

private:
	uint8_t count;
	uint8_t a[QUEUE_SIZE];
	uint8_t head;
	uint8_t tail;

public:
	/**
	 * The constructor takes no parameters as the queue size is not adjustable.
	**/
	Queue();
	/**
	 * Enqueue the data parameter to the end of the queue.
	 * @param data is of an uint8_t type
	 * @return void
	**/
	void enqueue(uint8_t data);
	/**
	 * Take the first item out of the queue and return it.
	 * @return uint8_t
	**/
	uint8_t dequeue();
	/**
	 * Returns the amount of items currently in the queue.
	 * @return uint8_t
	**/
	uint8_t get_count();

}; //queue

#endif //__QUEUE_H__
