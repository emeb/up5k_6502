/*
 * acia.h - C interface to the acia assembly routines
 * 03-04-19 E. Brombaugh
 */

#ifndef __ACIA__
#define __ACIA__

// low-level routines
extern void __fastcall__ acia_tx_str (char *str);
extern void __fastcall__ acia_tx_chr (char c);
extern char __fastcall__ acia_rx_chr (void);

// C routines
int acia_getc(void);

#endif
