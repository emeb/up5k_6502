/*
 * acia.c - interface routines for the ACIA
 * 03-05-19 E. Brombaugh
 */

#include "acia.h"
#include "fpga.h"

/*
 * ACIA handler called from the global ISR
 */
void acia_int(void)
{
	// echo
	acia_tx_chr(acia_rx_chr());
}

