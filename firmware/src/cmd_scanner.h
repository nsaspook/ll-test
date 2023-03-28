/* 
 * File:   cmd_scanner.h
 * Author: root
 *
 * Created on February 2, 2023, 8:23 AM
 */

#ifndef CMD_SCANNER_H
#define	CMD_SCANNER_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <stdarg.h>
#include <proc/p32mk0512mcj048.h>
#include "definitions.h"                // SYS function prototypes
#include "imu.h"

#define CMD_DRIVER "V1.701" 
#define CMD_ALIAS "COMMAND"

	/*
	 * serial port command parser
	 */
#define CLI_CMD_BUFFER_SIZE 128

	typedef struct _t_cmd {
		const char *cmd;
		void (*fh)(void*);
	} t_cmd;

	typedef struct _t_cli_ctx {
		t_cmd *cmds;
		char cmd[CLI_CMD_BUFFER_SIZE];
		uint8_t cpos;
	} t_cli_ctx;

#define CLI_IO_INPUT(__data) \
	linux_getc(__data)

#define CLI_IO_OUTPUT(__data, __len) \
	linux_putc(__data, __len)

#define KEY_CODE_BACKSPACE	0x7f
#define KEY_CODE_DELETE		0x7e
#define KEY_CODE_ENTER		'\r'
#define KEY_CODE_ESCAPE		0x1b

#define POSINC(__x) (((__x) < (CLI_CMD_BUFFER_SIZE - 1)) ? (__x + 1) : (__x))
#define POSDEC(__x) ((__x) ? ((__x) - 1) : 0)

	typedef enum _t_cmd_status {
		E_CMD_OK = 0,
		E_CMD_NOT_FOUND,
		E_CMD_TOO_SHORT,
		E_CMD_EMPTY
	} t_cmd_status;

	/*
	 * command parser functions
	 */
	void scmd_init(void);
	void cli_read(t_cli_ctx *);
	void dump_uart(void);

#ifdef	__cplusplus
}
#endif

#endif	/* CMD_SCANNER_H */

