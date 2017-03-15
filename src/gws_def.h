/*
 * gws_def.h
 *
 *  Created on: Feb 1, 2017
 *  Updated on: Mar 14, 2017
 *  Maintainer: Qige Zhao <qige@6harmonics.com>
 */

#ifndef GWS_DEF_H_
#define GWS_DEF_H_


// default use USE_GETOPT
#if (!defined(USE_GETOPT_LONG) && !defined(USE_GETOPT))
#define USE_GETOPT
#endif


// common define within GWS products
#define GWS_APP_KEY_LENGTH		16


// default values, define here
// TODO
//#define USE_GWS4K_RADIO
#define USE_GWS5K_RADIO


#endif /* GWS_DEF_H_ */
