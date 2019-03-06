/*
 * acia.c - interface routines for the ACIA
 * 03-05-19 E. Brombaugh
 */

#include <stdio.h>
#include "acia.h"
#include "fpga.h"

// only allow 2^N buffer sizes to make masking easy
#define ACIA_BUFSZ 128
#define ACIA_BUFMASK (ACIA_BUFSZ-1)

// ACIA buffers
char tx_buffer[ACIA_BUFSZ], rx_buffer[ACIA_BUFSZ];
unsigned char tx_wptr = 0, tx_rptr = 0, tx_n = 0,
				rx_wptr = 0, rx_rptr = 0, rx_n = 0, c;

/*
 * ACIA handler called from the global ISR
 */
void acia_irq_handler(void)
{
	// check for RX data
	if(ACIA_CTRL & ACIA_CTRL_RXF)
	{
		// get rx data
		c = acia_rx_chr();
		
		// check if there's room in the buffer
		if(rx_n <= ACIA_BUFSZ)
		{
			// save in buffer and update status
			rx_buffer[rx_wptr] = c;
			rx_wptr = (rx_wptr+1)&ACIA_BUFMASK;
			++rx_n;
		}
	}
}

/*
 * get a character from the RX buffer
 */
int acia_getc(void)
{
	int result;
	
	// check if data available
	if(rx_n)
	{
		// get char from buffer
		result = rx_buffer[rx_rptr];
		
		// update read ptr and count atomically
		asm("SEI");		// disable IRQ
		rx_rptr = (rx_rptr+1)&ACIA_BUFMASK;
		--rx_n;
		asm("CLI");		// enable IRQ
	}
	else
		result = EOF;
	
	return result;
}
