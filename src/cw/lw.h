#ifndef __LW_H
#define __LW_H

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

/**
 * @}
 */

#endif

