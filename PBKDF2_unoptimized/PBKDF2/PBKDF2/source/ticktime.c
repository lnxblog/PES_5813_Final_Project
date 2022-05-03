/**
 * @file    ticktime.c
 * @brief   Code to map SysTick timer into msec counter
 * @author  Howdy Pierce, howdy.pierce@colorado.edu
 */

#include "MKL25Z4.h"
#include "core_cm0plus.h"
#include "ticktime.h"
#include "cbfifo.h"

static volatile ticktime_t g_now = 0;
static ticktime_t g_timer = 0;

#if 0
void init_ticktime(void)
{
  // set control & status register to use 48 MHz/16 = 3 MHz
  // clock. Then interrupt 10000 times per second
  SysTick->LOAD = (48000000/16)/10000;
  NVIC_SetPriority(SysTick_IRQn, 3);
	NVIC_ClearPendingIRQ(SysTick_IRQn); 
	NVIC_EnableIRQ(SysTick_IRQn);	
  SysTick->VAL = 0;
  SysTick->CTRL &= ~(SysTick_CTRL_CLKSOURCE_Msk);
  SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;

  g_now = 0;
  g_timer = 0;
}

void SysTick_Handler()
{
	g_now++;
}

#endif

#if 0
void Init_TPM0(void)
{
	// Turn on clock to TPM
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;
	// Set clock source for tpm
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
	// Load the mod which divides the clock frequency
	TPM0->MOD = 2399;
	// Set TPM to enable DMA trigger, divide by 1 prescaler and enable counting (CMOD)
	TPM0->SC =  TPM_SC_CMOD(1) | TPM_SC_PS(0) | TPM_SC_TOIE_MASK;

	// Enable interrupts in NVIC
	NVIC_SetPriority(TPM0_IRQn, 2);
	NVIC_ClearPendingIRQ(TPM0_IRQn);
	NVIC_EnableIRQ(TPM0_IRQn);

}

void init_ticktime(void)
{
	g_now = 0;
	g_timer = 0;
	Init_TPM0();
}

void TPM0_IRQHandler(void)
{
  g_now++;
  TPM0->SC |= TPM_SC_TOIE_MASK;
}
#endif

ticktime_t now(void)
{
  return g_now;
}


void reset_timer(void)
{
  g_timer = g_now;
}


ticktime_t get_timer(void)
{
  return g_now - g_timer;
}
void delay(int ms)
{
	reset_timer();

	while(get_timer()<ms*10);
}
