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
 * Put a dword to to an output buffer. The dword
 * is converted to network byte order.
 *
 * @param dst destination buffer
 * @param dw dword to put
 * @return 4 (number of bytes put)
 */
#define lw_put_dword(dst,dw)\
	(*((uint32_t*)(dst)) = htonl(dw),4)

#define lw_set_byte(dst,b) \
	(*(dst)=b);

#define lw_set_word(dst,b) \
	(*((uint16_t*)(dst)) = htons(w))

#define lw_set_dword(dst,dw)\
	(*((uint32_t*)(dst)) = htonl(dw))


#define lw_get_byte(src)\
	(*(uint8_t*)(src))

#define lw_get_word(src) \
	(ntohs(  *((uint16_t*)(src))))

#define lw_get_dword(src) \
	(ntohl(  *((uint32_t*)(src))))




/* the following functions are defined as static inline and not as 
   macro to avoid any side effects */


/**
 * Put some bytes to an output buffer
 * @param dst Destination 
 * @param data Data to put
 * @len length of data (iini bytes)
 * @return The number of bytes put
 */
static inline int lw_put_data(uint8_t*dst,const uint8_t*data,uint16_t len)
{
	memcpy(dst,data,len); 
	return len;
}

/**
 * Put a #bstr_t to an output buffer
 * @param dst Destination 
 * @param b bstr to put
 * @return The number of bytes put
 */
static inline int lw_put_bstr(uint8_t * dst, const bstr_t b){
	lw_put_data(dst,bstr_data(b),bstr_len(b));
	return bstr_len(b);
}

/**
 * Put a #bstr16_t to an output buffer
 * @param dst Destination 
 * @param b bstr16 to put
 * @return The number of bytes put
 */
static inline int lw_put_bstr16(uint8_t * dst, const bstr16_t b){
	lw_put_data(dst,bstr16_data(b),bstr16_len(b));
	return bstr16_len(b);
}


/**
 * Put a string to an output buffer
 * @param dst Output buffer
 * @param str zero-terminated string to put
 * @return number of bytes put
 */
static inline int lw_put_str(uint8_t*dst,const uint8_t *str) {
	return lw_put_data(dst,str,strlen((char*)str));
}


static inline int lw_put_elem_hdr(uint8_t *dst,uint8_t type,uint16_t len)
{
	*dst=type;
	*((uint16_t*)(dst+1)) = htons(len);
	return 3;
}



extern uint16_t lw_checksum(uint8_t *d,int len);


/**
 * @}
 */

#endif

