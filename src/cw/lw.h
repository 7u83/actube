#ifndef __LW_H
#define __LW_H

/** 
 * @defgroup LW LWAPP Functions
 * @{
 */


#define lw_put_byte(dst,b) \
	(*(dst)=b,1)

#define lw_put_word(dst,w)\
	(*((uint16_t*)(dst)) = htons(w),2)

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

