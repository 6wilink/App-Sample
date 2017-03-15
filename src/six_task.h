/*
 * six_task.h
 *
 *  Created on: Jul 25, 2016
 *  Updated on: Mar 14, 2017
 *  Maintainer: Qige Zhao
 */

#ifndef SIX_TASK_H_
#define SIX_TASK_H_

#include "six_def.h"

// define your error code here
// return 0 means nothing goes wrong
enum TASK_ERR {
	TASK_OK = 0,
	TASK_ERR_INIT = 1,
	TASK_ERR_JOB = 100,
	TASK_ERR_RES = 200,
	TASK_ERR_IPC = 300
};

// start with user input
int Task(const void *env);

#endif /* SIX_TASK_H_ */
