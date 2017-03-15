/*
 * six_lbbuf.c
 *
 *  Created on: July 13, 2016
 *  Updated on: March 14, 2017
 *      Author: YY Wang, Qige
 *  Maintainer: Qige
 */
#include <stdio.h>
#include <string.h>
#include "six_lbbuf.h"


void lbbuf_init(LBBuf *lbbuf)
{
	memset(lbbuf, 0, sizeof(LBBuf));

	lbbuf->start = lbbuf->data;
	lbbuf->length = sizeof(lbbuf->data) - 1;

	lbbuf->data_head = lbbuf->start;
	lbbuf->data_length = 0;


	LBB_DBG("empty all data, reset all\n");
}

/*
 * save data (str/hex) to lbbuf
 * - test data length;
 * - test bytes available;
 * - calc basic counters;
 * - copy;
 */
int lbbuf_save(LBBuf *lbbuf, const byte *data, const uint data_length)
{
	//+ buffer length = data length + data available length
	uint lbbuf_length, lbbuf_data_length, lbbuf_da_length;
	lbbuf_length = lbbuf->length;
	lbbuf_data_length = lbbuf->data_length;
	lbbuf_da_length = lbbuf_length - lbbuf_data_length;

	if (lbbuf_da_length < 1)
		return LBB_ERR_CACHE_IS_FULL;
	if (data_length < 1)
		return LBB_ERR_DATA_INVALID;
	if (data_length > lbbuf_da_length)
		return LBB_ERR_DATA_TOOLONG;

	//+ buffer length = data head to start + data head to tail
	uint dh_to_start, dh_to_tail;
	dh_to_start = lbbuf->data_head - lbbuf->start;
	dh_to_tail = lbbuf_length - dh_to_start;

	//+ data available
	uint da_tail;
	if (dh_to_tail >= lbbuf_data_length) {
		da_tail = dh_to_tail - lbbuf_data_length;
	} else {
		da_tail = 0;
	}

	LBB_DBG("length/data length/bytes available = %d/%d/%d\n",
			lbbuf_length, lbbuf_data_length, lbbuf_da_length);
	LBB_DBG("data head/tail = %d/%d, bytes available in tail = %d\n",
			dh_to_start, dh_to_tail, da_tail);

	//+ save
	if (lbbuf_data_length > 0) {
		if (da_tail >= data_length) {
			memcpy(lbbuf->data_head + lbbuf_data_length, data, data_length);
		} else {

			/*memcpy(lbbuf->data_head + lbbuf_data_length, data, da_tail);
			if (lbbuf_data_length > dh_to_tail) {
				memcpy(lbbuf->start+(lbbuf_data_length-dh_to_tail), &data[da_tail], data_length - da_tail);
			} else {
				memcpy(lbbuf->start, &data[da_tail], data_length - da_tail);
			}*/

			if (da_tail > 0) {
				memcpy(lbbuf->data_head + lbbuf_data_length, data, da_tail);
			}

			memcpy(lbbuf->start, data + da_tail, data_length - da_tail);
		}
	} else {
		memcpy(lbbuf->start, data, data_length);
	}
	lbbuf->data_length += data_length;

	return LBB_OK;
}

/*
 * read loopback buffer data (ascii/hex)
 */
int lbbuf_read(LBBuf *lbbuf, byte *buff, uint *buff_length)
{
	if (*buff_length < 1)
		return LBB_ERR_DATA_INVALID;

	//+ cache length = data length + data available length
	uint lbbuf_length, lbbuf_data_length;
	lbbuf_length = lbbuf->length;
	lbbuf_data_length = lbbuf->data_length;

	//+ cache length = data head to start + data head to tail
	uint dh_to_head, dh_to_tail;
	dh_to_head = lbbuf->data_head - lbbuf->start;
	dh_to_tail = lbbuf_length - dh_to_head;

	//+ calc bytes to read
	int b2read = MIN(*buff_length, lbbuf_data_length);
	*buff_length = b2read;

	LBB_DBG("length/data length = %d/%d\n",
			lbbuf_length, lbbuf_data_length);
	LBB_DBG("data head/tail = %d/%d, bytes to read = %d\n",
			dh_to_start, dh_to_tail, b2read);

	if (dh_to_tail >= b2read) {
		memcpy(buff, lbbuf->data_head, b2read);
	} else {
		memcpy(buff, lbbuf->data_head, dh_to_tail);
		memcpy(&buff[dh_to_tail], lbbuf->start, b2read - dh_to_tail);
	}

#ifdef LBB_CLEAR_WHEN_READ
	lbbuf_move(lbbuf, *buff_length);
#endif

	return LBB_OK;
}

/*
 * discard data of b2move bytes
 * - data length <= b2move, init
 * -
 */
int lbbuf_move(LBBuf *lbbuf, uint move)
{
	if (move < 1)
		return LBB_OK;

	//+ cache length = data length + data available length
	unsigned int lbbuf_length, lbbuf_data_length;
	lbbuf_length = lbbuf->length;
	lbbuf_data_length = lbbuf->data_length;

	//+ cache length = data head to start + data head to tail
	unsigned int dh_to_head, dh_to_tail;
	dh_to_head = lbbuf->data_head - lbbuf->start;
	dh_to_tail = lbbuf_length - dh_to_head;

	LBB_DBG("length/data length = %d/%d\n",
			lbbuf_length, lbbuf_data_length);
	LBB_DBG("data head/tail = %d/%d, bytes to move = %d\n",
			dh_to_head, dh_to_tail, move);

	if (lbbuf_data_length > move) {
		if (dh_to_tail > move) {
#ifdef LBB_CLEAR_WHEN_MOVE
			int i;
			for(i = 0; i < move; i ++)
				*(lbbuf->data_head+i) = '\0';
#endif
			lbbuf->data_head += move;
		} else {
#ifdef LBB_CLEAR_WHEN_MOVE
			int i;
			for(i = 0; i <= dh_to_tail; i ++)
				*(lbbuf->data_head + i) = '\0';
			for(i = 0; i < move - dh_to_tail; i ++)
				*(lbbuf->start + i) = '\0';
#endif
			lbbuf->data_head = lbbuf->start + (move - dh_to_tail);
		}
		lbbuf->data_length -= move;
	} else {
		lbbuf_init(lbbuf);
	}

	return LBB_OK;
}

