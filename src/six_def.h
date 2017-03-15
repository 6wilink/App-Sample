/*
 * six_def.h
 *
 *  Created on: Feb 8, 2017
 *  Updated on: Mar 14, 2017
 *  Maintainer: Qige Zhao
 */

#ifndef DEF_H_
#define DEF_H_


// data types
typedef unsigned int			uint;
typedef unsigned char			uchar;
typedef char					byte;
typedef unsigned short 			ushort;


// call syslog()
#define LOG(fmt, ...) 	{ char _bf[1024] = {0}; snprintf(_bf, sizeof(_bf)-1, fmt, ##__VA_ARGS__); \
fprintf(stderr, "> %s", _bf); syslog(LOG_INFO, "%s", _bf); }

// -DRELEASE_DEBUG symbol to control debug msg
#ifdef RELEASE_DEBUG
#define DBG(fmt, ...)	{ fprintf(stderr, "dbg> "fmt, ##__VA_ARGS__); }
#else
#define DBG(fmt, ...)	{}
#endif


#endif /* DEF_H_ */
