#ifndef __LW_H
#define __LW_H

#include <stdint.h>
#include <arpa/inet.h>

#include "conn.h"
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

/** 
 * Same as #lw_set_byte, but w/o return value
 */
#define lw_set_byte(dst,b) \
	(*(dst)=b);

/**
 * Same as #lw_set_word, but no return value
 */
#define lw_set_word(dst,b) \
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
 * @return the byte red
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



/** 
 * Put a message element header for a message to contain a vendor specific payload
 * @param dst pointer to destination buffer
 * @param vendorid vendorid
 * @param elemid element id of vendor specific data
 * @len length of vendor specific data 
 * @return the number of bytes put (always 10)
 */
static inline int lw_put_vendor(uint8_t * dst, uint32_t vendorid,
					 uint16_t elemid, uint16_t len)
{
	lw_put_dword(dst + 0, vendorid);
	lw_put_word(dst + 4, elemid);
	return 6;
}






/**
 * @defgroup LWAPP_IN_HANDLER Input Handlers
 * @{
 */
int lw_in_vendor_specific(struct conn *conn, struct cw_action_in *a,
				  uint8_t * data, int len, struct sockaddr *from);

/**
 * @}
 */


/**
 * @defgroup LWAPP_MISC Misc Functions
 * @{
 */
extern uint16_t lw_checksum(uint8_t *d,int len);
extern int lw_put_sockaddr(uint8_t *dst, struct sockaddr_storage *addr);
/**
 * @}
 */


/**
 * @}
 */

#endif

