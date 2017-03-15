/*
 * six_app.c
 *
 *  Created on: Jul 25, 2016
 *  Updated on: Mar 14,, 2017
 *  Maintainer: Qige Zhao
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <getopt.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <syslog.h>

#include <signal.h>

#include "six_def.h"
#include "six_app.h"
#include "six_task.h"


static void app_version(void);
static void app_help(const char *app);
static void app_daemon(void);

/*
 * read user input
 * parse & save to app_conf
 * call main task()
 *
 * version 1.1.310117
 * todo: getopt(), getopt_long()
 */
int main(int argc, char **argv)
{
	// read & parse user cli input
	int ret = 0;
	int num;
	const char *app = argv[0];
	APP_ENV env;
	memset(&env, 0, sizeof(env));


#ifdef USE_GETOPT_LONG

	DBG("read command line params (getopt_long())\n");

	// verified @ 2017.02.15
	for( ; ; ) {
		int option_index = 0;
		int c = 0;
		static struct option long_options[] = {
				{ "help", 		no_argument, 0, 0 },
				{ "version", 	no_argument, 0, 0 },
				{ "debug", 		no_argument, 0, 0 },
				{ "daemon", 	no_argument, 0, 0 },
				{ "idle", 	required_argument, 0, 0 },
				{ 0, 			no_argument, 0, 0 }
		};

		c = getopt_long(argc, argv, "hvdDi:", long_options, &option_index);

		// no more params
		if ( c == -1 ) break;

		// unknown param
		if ( c == '?' ) {
			// TODO: handle your own parameters here
			continue;
		}

		// handle param
		switch(c) {
		case 0:
		case 'h':
			env.flag.help = 1;
			break;
		case 1:
		case 'v':
			env.flag.version = 1;
			break;
		case 2:
		case 'd':
			env.flag.debug = 1;
			break;
		case 3:
		case 'D':
			env.flag.daemon = 1;
			break;
		case 4:
		case 'i':
			env.conf.idle = atoi(optarg);
 			break;
		default: // running with default values
			break;
		}
	}

#else

	DBG("read command line params (getopt())\n");

	// verified by Qige @ 2017.01.31
	int c = 0;
	while((c = getopt(argc, argv, "vhdDi:b:")) != -1) {
		switch(c) {
		case 'h':
			env.flag.help = 1;
			break;
		case 'v':
			env.flag.version = 1;
			break;
		case 'd':
			env.flag.debug = 1;
			break;
		case 'D':
			env.flag.daemon = 1;
			break;
		case 'i':
			env.conf.idle = atoi(optarg);
			break;
		default:
			{
				// TODO: handle your own parameters here
			}
			break;
		}
	}
#endif


	DBG("check flags\n");
	if (env.flag.help) {
		//app_version();
		app_help(app);
		return 0;
	}

	if (env.flag.version) {
		app_version();
		return 0;
	}

	if (env.flag.debug) {
		app_version();
	}

	env.app.pid = getpid();
	snprintf(env.app.name, APP_LIMIT_COMMON_LENGTH, "%s", app);
	if (env.flag.daemon) {
		app_daemon();
		env.app.pid = getpid();
		LOG("running daemon (%s, pid=%d)\n", env.app.name, env.app.pid);
	} else {
		DBG("started (%s, pid=%d)\n", env.app.name, env.app.pid);
	}

	// call Task, hook functions
	ret = Task(&env);
	return ret;
}

// import from "mjpg-streamer"
// detach terminal, run in backgrund
static void app_daemon(void)
{
	int fr=0;

	fr = fork();
	if( fr < 0 ) {
		fprintf(stderr, "fork() failed\n");
		exit(1);
	}
	if ( fr > 0 ) {
		exit(0);
	}

	if( setsid() < 0 ) {
		fprintf(stderr, "setsid() failed\n");
		exit(1);
	}

	fr = fork();
	if( fr < 0 ) {
		fprintf(stderr, "fork() failed\n");
		exit(1);
	}
	if ( fr > 0 ) {
		//fprintf(stderr, "forked to background (%d)\n", fr);
		exit(0);
	}

	umask(0);

	chdir("/");
	close(0);
	close(1);
	close(2);

	open("/dev/null", O_RDWR);
	dup(0);
	dup(0);
}


static void app_version(void)
{
	printf("%s\n(%s)\n", APP_DESC, APP_VERSION);
}

static void app_help(const char *app)
{
#ifdef USE_GETOPT_LONG
	printf(" usage: %s [-D|--daemon] [-i|--idle sec]\n", app);
	printf("        %s [-d|--debug] [-v|--version|--ver] [-h|--help]\n\n", app);
#else
	printf("  usage: %s [-D] [-i sec]\n", app);
	printf("         %s [-d] [-v] [-h]\n\n", app);
#endif
	printf("     eg. %s -i5 -D\n", app);
}
