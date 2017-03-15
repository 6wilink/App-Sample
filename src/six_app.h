/*
 * six_app.h
 *
 *  Created on: Jul 25, 2016
 *  Updated on: Mar 14, 2017
 *  Maintainer: Qige Zhao
 */

#ifndef SIX_APP_H_
#define SIX_APP_H_

#include "gws_def.h"
#include "six_def.h"


#define APP_DESC				("App Sample")
#define APP_VERSION				("v1.0-1-140317. Buildtime: "__DATE__", "__TIME__)


// size define
#define APP_LIMIT_COMMON_LENGTH			32


// user input struct
// TODO: change this struct to fit your need
// save all user input into one struct
// and pass its pointer to task/job/work
typedef struct {
	struct {
		int version;
		int help;
		int debug;
		int daemon;
	} flag;

	struct {
		int pid;
		char name[APP_LIMIT_COMMON_LENGTH];
		int idle;
	} app;

	struct {
		char key[APP_LIMIT_COMMON_LENGTH];
	} license;

	struct {
		int idle;
	} conf;

} APP_ENV;

#endif /* SIX_APP_H_ */
