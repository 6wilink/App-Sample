/*
 * six_task.c
 *
 *  Created on: Jul 25, 2016
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
#include "six_task.h"

// Hardware Independence Here
#ifdef USE_GWS4K_RADIO
#include "gws4k_task.h"
#else
#include "gws5k_task.h" // default radio/hw/platform
#endif


// Hook functions when calling
static void task_init(void);

// Answer Signals
// call Hook function (*task_signal)(void);
static void task_prepare_exit(void);


// hook different function for Hardware Independence
int (*task_run)(const void *);

// handle signal SIGTERM/SIGQUIT/SIGINT
void (*task_signal)(void);


// MAIN Calling sequences.
// - init()
// - run()
//
// if you want to fork a process, do it here;
int Task(const void *env)
{
	int ret;

	task_init();
	ret = (*task_run)(env);

	return ret;
}


// set function pointer
// handle signal()
static void task_init(void)
{
	// hook function
#ifdef USE_GWS4K_RADIO
	task_run = &gws4k_task_Run;
	task_signal = &gws4k_task_Signal;
#else
	task_run = &gws5k_task_Run;
	task_signal = &gws5k_task_Signal;
#endif

	// release before interrupt/quit/terminated
	DBG("handling signal(SIGINT, SIGQUIT, SIGTERM)\n");
	signal(SIGINT, 	(__sighandler_t) task_prepare_exit); //+ "^C"
	signal(SIGQUIT, (__sighandler_t) task_prepare_exit); //+ "^\"
	signal(SIGTERM,	(__sighandler_t) task_prepare_exit); //+ "kill", not "kill -9"

	// release zombies
	// comment next line if "wait()/waitpid()"
	DBG("handling signal(SIGCHLD): SIG_IGN\n");
	signal(SIGCHLD, SIG_IGN);
}


// call & reset signal(), tell your real executor, it's time to exit
// so hit ^c twice will interrupt right away
static void task_prepare_exit(void)
{
	LOG("signal(SIGINT | SIGQUIT | SIGTERM) detected, exiting...\n");
	(*task_signal)();

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGTERM, SIG_DFL);
}
