/* 
 * File:   remote_cmd.h
 * Author: root
 *
 * Created on March 2, 2023, 10:11 AM
 */

#ifndef REMOTE_CMD_H
#define	REMOTE_CMD_H

#ifdef	__cplusplus
extern "C" {
#endif
#include "imupic32mcj.h"
#include "cmd_scanner.h"
#include "host.h"

#define REMOTE_DRIVER "V1.001" 
#define REMOTE_ALIAS "REMOTE CMD"

	/*
	 * command execute functions
	 * prototypes
	 */
	void fh_show_idle(void *a_data);
	void fh_stop_trigger(void *a_data);
	void fh_start_trigger(void *a_data);
	void fh_start_safe(void *a_data);
	void fh_start_lock(void *a_data);
	void fh_start_unlock(void *a_data);
	void fh_start_warn(void *a_data);
	void remote_cmd_decode(imu_host_t *);

	extern void fh_start_AT(void *a_data);

	extern char response_buffer[RBUFFER_SIZE];
	extern char cmd_buffer[FBUFFER_SIZE];
	extern imu_host_t host0;
	extern t_cmd g_cmds[];
	extern char hbuffer[FBUFFER_SIZE];
	extern imu_cmd_t imu0;

#ifdef	__cplusplus
}
#endif

#endif	/* REMOTE_CMD_H */

