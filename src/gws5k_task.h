/*
 * gws5k_task.h
 *
 *  Created on: Mar 14, 2017
 *  Updated on: Mar 14, 2017
 *  Maintainer: Qige Zhao
 */

#ifndef GWS5K_TASK_H_
#define GWS5K_TASK_H_

// include if needed
//#include "six_def.h"
//#include "gws_def.h"

#define GWS5K_TASK_DFL_IDLE 1


// define your error code here
// return 0 means nothing goes wrong
enum GWS5K_ERR {
	GWS5K_OK = 0,
	GWS5K_ERR_INIT = 1
};

// start with user input
// notice that all config maybe equal to "0"
// hook different function for Hardware Independence
int gws5k_task_Run(const void *env);

// handle signal SIGTERM/SIGQUIT/SIGINT
void gws5k_task_Signal(void);

#endif /* GWS5K_TASK_H_ */
