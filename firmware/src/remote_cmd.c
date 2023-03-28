/*
 * decode and send the IP remote host message
 * to the sensor for execution
 */

#include "remote_cmd.h"

char hbuffer[FBUFFER_SIZE];

t_cmd g_cmds[] = {

	{ "I", fh_show_idle},
	{ "D", fh_stop_trigger},
	{ "E", fh_start_trigger},
	{ "A", fh_start_AT},
	{ "W", fh_start_warn},
	{ "U", fh_start_unlock},
	{ "L", fh_start_lock},
	{ "S", fh_start_safe},

	// null command terminator
	{ 0x00, 0x00}
};

void fh_show_idle(void *a_data)
{
	snprintf(cmd_buffer, max_buf, "Show Link Status            ");
	host0.cmd = CMD_ACK;
	host0.secret = 0;
}

void fh_stop_trigger(void *a_data)
{
	snprintf(cmd_buffer, max_buf, "Stop Trigger                ");
	host0.cmd = CMD_SAFE;
	host0.secret = 0;
}

void fh_start_trigger(void *a_data)
{
	snprintf(cmd_buffer, max_buf, "Start Trigger               ");
	host0.cmd = CMD_SPIN_DOWN;
	host0.secret = 0;
}

void fh_start_warn(void *a_data)
{
	snprintf(cmd_buffer, max_buf, "Start Warning               ");
	host0.cmd = CMD_WARN_ON;
	host0.secret = 0;
}

void fh_start_unlock(void *a_data)
{
	snprintf(cmd_buffer, max_buf, "Unlock System               ");
	host0.cmd = CMD_UNLOCK;
	host0.secret = HOST_SECRET;
}

void fh_start_lock(void *a_data)
{
	snprintf(cmd_buffer, max_buf, "Lock System                 ");
	host0.cmd = CMD_LOCK;
	host0.secret = 0;
}

void fh_start_safe(void *a_data)
{
	snprintf(cmd_buffer, max_buf, "Safe System                 ");
	host0.cmd = CMD_SAFE;
	host0.secret = 0;
}

/*
 * decode and execute the IP remote message
 * from the sensor host
 */
void remote_cmd_decode(imu_host_t * host_ptr)
{
	/*
	 * decode received host message
	 */
	snprintf(hbuffer, max_buf, "Host CPU %llX , Cmd %i", host_ptr->host_serial_id, host_ptr->cmd);
	switch (host_ptr->cmd) {
	case CMD_ACK:
	case CMD_IDLE:
		break;
	case CMD_SPIN_DOWN: // vibration action triggered
//		PWM1EN_Set();
		if (!imu0.locked) {
//			PWM4EN_Set();
			imu0.down = true;
			imu0.locked = true; // auto relock
		}
		break;
	case CMD_LOCK:
		imu0.locked = true;
		break;
	case CMD_UNLOCK:
		if (host_ptr->secret == HOST_SECRET) {
			imu0.locked = false;
			host_ptr->secret = 0; //clear
			host_ptr->cmd = CMD_IDLE;
		} else {
			snprintf(cmd_buffer, max_buf, "Unlock System Failed        ");
		}
		break;
	case CMD_WARN_ON: // vibration warning triggered
//		PWM1EN_Set();
		imu0.warn = true;
		break;
	case CMD_WARN_OFF:
//		PWM1EN_Clear();
		imu0.warn = false;
		break;
	case CMD_SAFE:
//		PWM1EN_Clear();
//		PWM4EN_Clear();
		imu0.warn = false;
		imu0.down = false;
		break;
	default:
		snprintf(hbuffer, max_buf, "Host CPU %llX", host_ptr->host_serial_id);
		imu0.locked = true;
		break;
	}
}