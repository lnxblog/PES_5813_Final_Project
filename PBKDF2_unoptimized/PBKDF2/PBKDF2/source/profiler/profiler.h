/*
 * profiler.h
 *
 *  Created on: May 1, 2022
 *      Author: hemanth
 */

#ifndef PROFILER_H_
#define PROFILER_H_
#include "MKL25Z4.h"

/*
 * start_profiler
 * Configures systick interrupt to trigger at PC sample frequency
 */
void start_profiler();

/*
 * stop_profiler
 * Disables the systick interrupt
 */
uint32_t stop_profiler();

#endif /* PROFILER_H_ */
