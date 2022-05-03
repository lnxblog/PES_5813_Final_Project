/*
 * uart.c
 *
 *  Created on: Mar 25, 2022
 *      Author: hemanth
 */

#include "MKL25Z4.h"
#include "cbfifo.h"
#include <string.h>
#define SYS_CLOCK 24000000UL
#define UART_OVERSAMPLE_RATE 16
#define BAUD_RATE 115200
#define DATA_8_BIT 0
#define PARITY_ENABLE 0
#define STOP_BITS_TWO 1

/*
 * see uart.h for details
 */
void Init_UART0() {
	uint16_t sbr;

	// Enable clock gating for UART0 and Port A
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;

	// Make sure transmitter and receiver are disabled before init
	UART0->C2 &= ~UART0_C2_TE_MASK & ~UART0_C2_RE_MASK;

	// Set UART clock to FLL clock
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);
	SIM->SOPT2 &= ~SIM_SOPT2_PLLFLLSEL_MASK;

	// Set pins to UART0 and Tx
	PORTA->PCR[2] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Tx

	// Set baud rate and oversampling ratio
	sbr = (uint16_t)((SYS_CLOCK)/(BAUD_RATE * UART_OVERSAMPLE_RATE));
	UART0->BDH &= ~UART0_BDH_SBR_MASK;
	UART0->BDH |= UART0_BDH_SBR(sbr>>8);
	UART0->BDL = UART0_BDL_SBR(sbr);
	UART0->C4 |= UART0_C4_OSR(UART_OVERSAMPLE_RATE-1);

	// Disable interrupts for RX active edge and LIN break detect, select two stop bit
	UART0->BDH |= UART0_BDH_RXEDGIE(0) | UART0_BDH_SBNS(STOP_BITS_TWO) | UART0_BDH_LBKDIE(0);

	// Don't enable loopback mode, use 8 data bit mode, don't use parity
	UART0->C1 = UART0_C1_LOOPS(0) | UART0_C1_M(DATA_8_BIT) | UART0_C1_PE(PARITY_ENABLE);
	// Don't invert transmit data, don't enable interrupts for errors
	UART0->C3 = UART0_C3_TXINV(0) | UART0_C3_ORIE(0)| UART0_C3_NEIE(0)
			| UART0_C3_FEIE(0) | UART0_C3_PEIE(0);

	// Clear error flags
	UART0->S1 = UART0_S1_OR(1) | UART0_S1_NF(1) | UART0_S1_FE(1) | UART0_S1_PF(1);

	// Try it a different way
	UART0->S1 |= UART0_S1_OR_MASK | UART0_S1_NF_MASK | UART0_S1_FE_MASK | UART0_S1_PF_MASK;

	// Send LSB first, do not invert received data
	UART0->S2 = UART0_S2_MSBF(0) | UART0_S2_RXINV(0);

	// initialize queue
	init_fifo(WR_Q);

	// setup UART interrupt handler
	NVIC_SetPriority(UART0_IRQn, 3);
	NVIC_ClearPendingIRQ(UART0_IRQn);
	NVIC_EnableIRQ(UART0_IRQn);

	// Enable transmit interrupts
	UART0->C2 |= UART_C2_TIE(1) ;

	// Enable UART transmitter
	UART0->C2 |= UART0_C2_TE(1);
}

/*
 * Interrupt handler for UART receive full and transmit empty flags
 *
 */

void UART0_IRQHandler(void)
{
	uint8_t ch;

	// check for any errors
	if (UART0->S1 & (UART_S1_OR_MASK |UART_S1_NF_MASK | UART_S1_FE_MASK | UART_S1_PF_MASK)) {
		// clear the error flags
		UART0->S1 |= UART0_S1_OR_MASK | UART0_S1_NF_MASK | UART0_S1_FE_MASK | UART0_S1_PF_MASK;
		// read the data register to clear RDRF
		ch = UART0->D;
	}
	// check if transfer-register-empty flag set by interrupt
	if (UART0->S1 & UART0_S1_TDRE_MASK) {
		// ready to transmit. pop character from write queue
		if(cbfifo_dequeue(WR_Q,&ch)==1)
		{
			UART0->D = ch;
		}
		else
		{
			//disable Transmit interrupt
			UART0->C2 &= ~UART_C2_TIE_MASK;
		}
	}
}
/*
 *	I/O function for write call
 *	Used by upper level API like printf to redirect to stdout
 *
 *	Parameters:
 *	int: Not used. Default stdout
 *	char* pointer to buffer to write out to stdout queue
 *	int number of bytes to write
 *
 *	Return:
 *	-1 error
 *	 0 success
 */

int __sys_write(int handle, char *buf, int size)
{
	int i=0,bytes,ret=0;
	while(i<size)
	{

		bytes=cbfifo_enqueue(WR_Q,(void*)buf,size-i);
		if(bytes==(size_t)-1)
		{
			ret=-1;
			break;
		}

		buf += bytes;
		i += bytes;
		UART0->C2 |= UART_C2_TIE(1);
	}
	return ret;
}

