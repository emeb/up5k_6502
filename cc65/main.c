/*
 * main.c - top level 6502 C code for up5k_6502
 * 03-05-19 E. Brombaugh
 * based on example code from https://cc65.github.io/doc/customizing.html
 */
 
#include <stdio.h>
#include "fpga.h"
#include "acia.h"

char *msg = "\n\n\rup5k 6502 cc65 serial test\n\n\r";
unsigned long cnt;
unsigned char x = 0;
int rx_chr;

int main()
{
	//  Send startup message
	acia_tx_str(msg);
	
	// enable ACIA IRQ
	ACIA_CTRL = 0x80;
	asm("CLI");
	
    // Run forever
    while(1)
    {
		// delay
		cnt = 4096L;
		while(cnt--)
		{
		}
		
        // write counter msbyte to GPIO
        GPIO_DATA = x;
        x++;
		
		// handle serial echo
		if((rx_chr = acia_getc()) != EOF)
			acia_tx_chr(rx_chr);
    }

    //  We should never get here!
    return (0);
}
