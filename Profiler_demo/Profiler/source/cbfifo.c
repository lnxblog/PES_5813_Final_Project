/**
* @file cbfifo.c
* @brief circular buffer FIFO implementation
*
* cbfifo.c definitions for circular buffer FIFO operations.
*
* @author Hemanth Nandish
* @date January 21 2022
* @version 1.0
*
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "MKL25Z4.h"
#include "cbfifo.h"
#define CAPACITY 1024
#define NUM_FIFO 1
typedef struct cbfifo_t
{
	char buffer[CAPACITY];
	int rd_idx;
	int wr_idx;
	bool full;
} cbfifo_s;

static cbfifo_s g_fifos[NUM_FIFO];

/*
 * see cbfifo.h for information
 */
void init_fifo(int idx)
{
	memset(&g_fifos[idx],0,sizeof(cbfifo_s));
}

/*
 *  see cbfifo.h for information
 */

size_t cbfifo_enqueue(int idx,void *buf, size_t nbyte)
{
	size_t ret;
	uint32_t masking_state;
	if(buf == NULL)
		ret=(size_t)-1;

	// save current masking state
	masking_state = __get_PRIMASK();
	// disable interrupts
	__disable_irq();

	if(g_fifos[idx].full || nbyte==0)
		ret=0;
	else
	{
		long length,capacity;
		int limit;
		length = cbfifo_length(idx);
		capacity = cbfifo_capacity();
		if(capacity-length < nbyte)
			limit = capacity - length;
		else
			limit = nbyte;

		int i=0;
		while(i < limit)
		{
			g_fifos[idx].buffer[(g_fifos[idx].wr_idx+i)&(CAPACITY-1)]=*((char*)buf+i);
			i++;
		}

		g_fifos[idx].wr_idx = (g_fifos[idx].wr_idx + limit)&(CAPACITY-1);

		if(length+limit==CAPACITY)
			g_fifos[idx].full=true;
		ret=limit;
	}
	// restore  interrupt masking state
	__set_PRIMASK(masking_state);

	return ret;
}

/*
 *  cbfifo_dequeue: see cbfifo.h for information
 */

size_t cbfifo_dequeue(int idx,uint8_t *byte)
{
	uint32_t masking_state,ret;

	// save current masking state
	masking_state = __get_PRIMASK();
	// disable interrupts
	__disable_irq();

	if (!g_fifos[idx].full && (g_fifos[idx].rd_idx==g_fifos[idx].wr_idx)) // check if queue empty
		ret=0;
	else
	{
		ret=1;
		*byte=g_fifos[idx].buffer[g_fifos[idx].rd_idx];		// read byte
		g_fifos[idx].rd_idx = (g_fifos[idx].rd_idx + 1)&(CAPACITY-1);	// update read index
		g_fifos[idx].full=false;	// queue is not full now
	}
	// restore interrupt masking state
	__set_PRIMASK(masking_state);
	return ret;
}

/*
 *  cbfifo_length: see cbfifo.h for information
 */
size_t cbfifo_length(int idx)
{
	if(g_fifos[idx].full)
	return CAPACITY;
	return (g_fifos[idx].wr_idx-g_fifos[idx].rd_idx)&(CAPACITY-1);
}

/*
 *  cbfifo_capacity: see cbfifo.h for information
 */
size_t cbfifo_capacity()
{
	return CAPACITY;
}

