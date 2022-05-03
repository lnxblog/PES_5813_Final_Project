/*
 * timer.c
 *
 *  Created on: Apr 22, 2022
 *      Author: hemanth
 */

#include "cbfifo.h"
#include "MKL25Z4.h"
#define CORE_FREQ 24000000L
#define WORD_OFFSET 11
#define PC_SAMPLE_HZ 1440

uint32_t samples;

void start_profiler()
{
	SysTick_Config(CORE_FREQ/PC_SAMPLE_HZ); // Trigger systick to begin sampling PC at every interrupt
	samples=0;
}

void SysTick_Handler()
{
	int var;
	int ptr;
	ptr=*(&var + WORD_OFFSET); // Get return address from the stack
	char *buf = (char*)&ptr;
	int i=0;
	buf[i+3]=0xe3;  // add non ascii delimiter to sampled address. Required during parsing
	samples++;
	while(i<4)
	{
		cbfifo_enqueue(WR_Q,(void*)buf,4);	// push to UART queue
		i++;
	}

}

uint32_t stop_profiler()
{
	SysTick->CTRL = 0; // disable systick interrupts to stop profiling
	return samples;
}
