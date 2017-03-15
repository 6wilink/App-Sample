/*
 * six_lbbuf.h
 *
 *  Created on: July 13, 2016
 *  Updated on: March 14, 2017
 *      Author: YY Wang, Qige
 *  Maintainer: Qige
 */

#ifndef SIX_LBBUF_H_
#define SIX_LBBUF_H_

// data types
typedef unsigned int			uint;
typedef unsigned char			uchar;
typedef char					byte;
typedef unsigned short 			ushort;

#define MIN(x, y)					(x < y ? x : y)

// empty debug print out
#ifdef _ENABLE_LBBUF_DEBUG
#define LBB_DBG(format, ...)		{printf("lbbuf > "format, ##__VA_ARGS__);}
#else
#define LBB_DBG(fmt, ...)			{}
#endif


// features
#define LBB_CLEAR_WHEN_MOVE
#define LBB_CLEAR_WHEN_READ

// @returns
enum LBB_ERR {
	LBB_OK = 0,
	LBB_ERR_CACHE_IS_FULL = 1,
	LBB_ERR_DATA_INVALID = 100,
	LBB_ERR_DATA_TOOLONG
};


#ifdef DEBUG_LBBUF
#define LBB_CONF_BUF_LENGTH		16
#else
#define LBB_CONF_BUF_LENGTH		1024
#endif

typedef struct {
	char *start;
	uint length;
	char *data_head;
	uint data_length;
	byte data[LBB_CONF_BUF_LENGTH];
} LBBuf;


/*
 * init loopback buffer
 */
void lbbuf_init(LBBuf *lbbuf);

/*
 * return bytes that saved
 * return value < 0, error number
 */
int lbbuf_save(LBBuf *lbbuf, const byte *data, const uint data_length);

/*
 * return bytes that read from buffer
 * return value < 0, error number
 */
int lbbuf_read(LBBuf *lbbuf, byte *buff, uint *buff_length);

/*
 * return bytes that buffer discarded
 * return value = 0, init loopback buffer when "move" > "data_length"
 * return value < 0, error number
 */
int lbbuf_move(LBBuf *lbbuf, uint move);


#endif /* SIX_LBBUF_H_ */
