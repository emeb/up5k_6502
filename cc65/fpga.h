/*
 * fpga.h - handy info about the FPGA
 * 03-04-19 E. Brombaugh
 */

#ifndef __FPGA__
#define __FPGA__

// registers
#define GPIO_DATA (*(unsigned char *) 0xD000)
#define ACIA_CTRL (*(unsigned char *) 0xE000)
#define ACIA_DATA (*(unsigned char *) 0xE001)

// bits
#define ACIA_CTRL_RXF 0x01
#define ACIA_CTRL_TXE 0x02
#define ACIA_CTRL_ERR 0x10
#define ACIA_CTRL_IRQ 0x80

#endif
