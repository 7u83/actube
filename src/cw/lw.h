#ifndef __LW_H
#define __LW_H

#include <stdint.h>
#include <arpa/inet.h>

#include "bstr.h"



/** 
 * @defgroup LW LWAPP Functions
 * @{
 */

/**
 * Put a byte to to an output buffer 
 *
 * @param dst destination buffer
 * @param b byte to put
 * @return 1 (number of bytes put)
 */
#define lw_put_byte(dst,b) \
	(*(dst)=b,1)

/**
 * Put a word to to an output buffer. The word
 * is converted to network byte order.
 *
 * @param dst destination buffer
 * @param w word to put
 * @return 2 (number of bytes put)
 */
#define lw_put_word(dst,w)\
	(*((uint16_t*)(dst)) = htons(w),2)

/**
 * Put a dword to an output buffer. The dword
 * is converted to network byte order.
 *
 * @param dst destination buffer
 * @param dw dword to put
 * @return 4 (number of bytes put)
 */
#define lw_put_dword(dst,dw)\
	(*((uint32_t*)(dst)) = htonl(dw),4)

/** 
 * Same as #lw_set_byte, but w/o return value
 */
#define lw_set_byte(dst,b) \
	(*(dst)=b)

/**
 * Same as #lw_set_word, but no return value
 */
#define lw_set_word(dst,w) \
	(*((uint16_t*)(dst)) = htons(w))

/**
 * Same as #lw_put_dword, but the return value
 * is unspecified.
 */
#define lw_set_dword(dst,dw)\
	(*((uint32_t*)(dst)) = htonl(dw))

/**
 * Read a byte from input buffer
 * @param src Pointer to input buffer
 * @return the byte reed
 */
#define lw_get_byte(src)\
	(*(uint8_t*)(src))

/** 
 * Read a word from input buffer and convert it from 
 * network format to local format.
 * @param src Pointer to input buffer
 * @return word 
 */
#define lw_get_word(src) \
	(ntohs(  *((uint16_t*)(src))))

/**
 * Read a dword from input buffer and convert it from
 * network format to local
 * @param src Pointer to input bufffer
 * @return the dword red
 */
#define lw_get_dword(src) \
	(ntohl(  *((uint32_t*)(src))))



int lw_put_data(uint8_t*dst,const uint8_t*data,uint16_t len);
int lw_put_bstr(uint8_t * dst, const bstr_t b);
int lw_put_bstr16(uint8_t * dst, const bstr16_t b);
int lw_put_str(uint8_t*dst,const uint8_t *str);
int lw_put_elem_hdr(uint8_t *dst,uint8_t type,uint16_t len);
int lw_put_vendor(uint8_t * dst, uint32_t vendorid,
					 uint16_t elemid, uint16_t len);


/*
int lw_in_vendor_specific(struct conn *conn, struct cw_action_in *a,
				  uint8_t * data, int len, struct sockaddr *from);
*/


uint16_t lw_checksum(uint8_t *d,int len);

#define lw_foreach_elem(d,msg,len) for(d=msg; d<msg+len; d=d+3+LWMSGELEM_GET_LEN(d))

/**
 * @} LW
 */

#endif

