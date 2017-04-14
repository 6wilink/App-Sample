/*
 * gws5k_task.c
 *
 *  Created on: Mar 14, 2017
 *  Updated on: Mar 14, 2017
 *  Maintainer: Qige Zhao
 */

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#include <syslog.h>

#include <sys/ipc.h>
#include <sys/shm.h>

#include "six_def.h"
#include "six_app.h"
#include "gws5k_task.h"


int FLAG_TASK_EXIT = 0;

// MAIN Calling sequences.
// - init()
// - run()
//
// if you want to fork a process, do it here;
int gws5k_task_Run(const void *env)
{
	APP_ENV *conf = (APP_ENV *) env;
	int idle = conf->conf.idle ? conf->conf.idle : GWS5K_TASK_DFL_IDLE;
	for( ; ; ) {
		if ( FLAG_TASK_EXIT ) {
			break;
		}


		sleep(idle);
	}
	return GWS5K_OK;
}


// call & reset signal(), tell your real executor, it's time to exit
// so hit ^c twice will interrupt right away
void gws5k_task_Signal(void)
{
	FLAG_TASK_EXIT = 1;
}
