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
static char* Last = 0;

/*
 * try local tokenizer
 */
char* my_strtok (char* s1, const char s2)
{
    char c;
    char* start;

    /* Use the stored location if called with a NULL pointer */
    if (s1 == 0) {
        s1 = Last;
    }

    /* If s1 is empty, there are no more tokens. Return 0 in this case. */
    if (*s1 == '\0') {
        return 0;
    }

    /* Search the address of the first element in s1 that equals none
    ** of the characters in s2.
    */
    while ((c = *s1) && (c == s2)) {
        ++s1;
    }
    if (c == '\0') {
        /* No more tokens found */
        Last = s1;
        return 0;
    }

    /* Remember the start of the token */
    start = s1;

    /* Search for the end of the token */
    while ((c = *s1) && (c != s2)) {
        ++s1;
    }
    if (c == '\0') {
        /* Last element */
        Last = s1;
    } else {
        *s1 = '\0';
        Last = s1 + 1;
    }

    /* Return the start of the token */
    return start;
}


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
