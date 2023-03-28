#include "remote_cmd.h"
#include "cmd_scanner.h"

t_cli_ctx cli_ctx; // command buffer 
uint8_t res = E_CMD_OK;

static void cli_init(t_cli_ctx *a_ctx, t_cmd *a_cmds)
{
	memset(a_ctx, 0x00, sizeof(t_cli_ctx));
	a_ctx->cmds = a_cmds;
}

void scmd_init(void)
{
	cli_init(&cli_ctx, g_cmds);
}

uint8_t linux_getc(uint8_t *a_data)
{
	if (UART1_ReceiverIsReady()) {
		UART1_Read(a_data, 1);
		return 1;
	} else {
		return 0;
	}
}

uint8_t linux_putc(uint8_t *data, uint8_t a_len)
{
	return 1;
}

static uint8_t _cli_interpret_cmd(t_cli_ctx *a_ctx)
{
	uint8_t i = 0;
	uint8_t ret = E_CMD_OK;

	if (!strlen(a_ctx->cmd)) {
		return E_CMD_EMPTY;
	}

	if (strlen(a_ctx->cmd) < 1) {
		return E_CMD_TOO_SHORT;
	}


	while (a_ctx->cmds[i].fh) {
		if (!strncmp(a_ctx->cmds[i].cmd, a_ctx->cmd, strlen(a_ctx->cmds[i].cmd))) {
			// call the handler
			a_ctx->cmds[i].fh((void *) a_ctx);
			break;
		}
		i++;
	}

	if (!a_ctx->cmds[i].fh) {
		ret = E_CMD_NOT_FOUND;
	}

	return ret;
}

void cli_read(t_cli_ctx *a_ctx)
{
	uint8_t i = 0x00;

	// if no character available - then exit
	if (UART1_ReceiverIsReady()) {
		i = UART1_ReadByte();
	} else {
		return;
	}


	/// char by char matching
	switch (i) {
	case KEY_CODE_BACKSPACE: // backspace
	case KEY_CODE_DELETE: // del
		break;
	case KEY_CODE_ESCAPE: // special characters
		break;
	case KEY_CODE_ENTER: // new line
		a_ctx->cmd[POSINC(a_ctx->cpos)] = '\0';
		res = _cli_interpret_cmd(a_ctx);
		a_ctx->cpos = 0;
		memset(a_ctx->cmd, 0x00, CLI_CMD_BUFFER_SIZE);
		break;
	default:
		/* echo */
		if (a_ctx->cpos < (CLI_CMD_BUFFER_SIZE - 1)) {
			a_ctx->cmd[a_ctx->cpos++] = i;
		}
		break;
	}
}

void dump_uart(void)
{
	while (UART1_ReceiverIsReady()) {
		UART1_ReadByte();
	}
}