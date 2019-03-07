/*
 * cmd.c - process commands
 * 03-06-19 E. Brombaugh
 */

#include <stdio.h>
#include <string.h>
#include "cmd.h"
#include "acia.h"

#define MAX_CMD_LEN 128
#define MAX_ARGS 4

char cmd_buff[MAX_CMD_LEN];
unsigned char cmd_widx;

/*
 * emit the command prompt
 */
void cmd_prompt(void)
{
	cmd_widx = 0;
	acia_puts("Command>");
}

/*
 * handle a command line
 */
void cmd_parse(void)
{
	char *token, *argv[MAX_ARGS];
	int argc;

	// debugging
	acia_puts(" ->");
	acia_puts(cmd_buff);
	acia_puts("\n\r");
	
	// parse out three tokens: cmd arg arg
	argc = 0;
	token = strtok(cmd_buff, " ");
	while((token != NULL) && (argc < MAX_ARGS))
	{
		argv[argc] = token;
		
		// debugging
		acia_putc(argc + '0');
		acia_puts(" ->");
		acia_puts(token);
		acia_puts("\n\r");

		++argc;
		token = strtok(NULL, " ");
	}
}

/*
 * initialize the command processor
 */
void cmd_init(void)
{
	cmd_prompt();
}

/*
 * fetch characters and process
 */
void cmd_do(void)
{
	int rx_chr;
	
	if((rx_chr = acia_getc()) != EOF)
	{
		if(rx_chr=='\b')
		{
			if(cmd_widx)
			{
				acia_puts("\b \b"); // erase & backspace
				--cmd_widx;
			}
		}
		else if(rx_chr=='\r')
		{
			acia_puts("\r\n"); // add LF for CR
			cmd_buff[cmd_widx] = 0;	// mark end of command
			cmd_parse();
			cmd_prompt();
		}
		else
		{
			if(cmd_widx<(MAX_CMD_LEN-1))
			{
				// echo and save character in buffer
				acia_putc(rx_chr);
				cmd_buff[cmd_widx] = rx_chr;
				++cmd_widx;
			}
		}
	}
}
