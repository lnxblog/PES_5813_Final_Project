/*
 * cbfifo.h - a fixed-size FIFO implemented via a circular buffer
 *
 * Author: Hemanth Nandish
 *
 */

#ifndef _CBFIFO_H_
#define _CBFIFO_H_

#include <stdlib.h>  // for size_t
#include <stdint.h>
#include <stdbool.h>

// index of queue
#define WR_Q 0


// size of queue
#define CAPACITY 1024
/*
 * zero the queue structure using memset
 *
 * Parameters:
 *  int   index of queue
 */
void init_fifo(int);

/*
 * Enqueues a byte on the queue
 *
 * Parameters:
 *   int index of queue
 *   uint8_t     byte to enqueue
 *
 * Returns:
 *   1 if byte enqueued else 0
 */
size_t cbfifo_enqueue(int,void *, size_t);


/*
 * Attempts to dequeue a byte from the queue
 *
 * Parameters:
 *   int index of queue
 *   uint8_t*     location to store byte
 *
 * Returns:
 *   1 if byte dequeued else 0
 *
 */
size_t cbfifo_dequeue(int, uint8_t*);


/*
 * Returns the number of bytes currently on the FIFO.
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   Number of bytes currently available to be dequeued from the FIFO
 */
size_t cbfifo_length(int);


/*
 * Returns the FIFO's capacity
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   The capacity, in bytes, for the FIFO
 */
size_t cbfifo_capacity();

#endif // _CBFIFO_H_
