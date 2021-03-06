
#include "lw.h"

/**
 * Put some bytes to an output buffer
 * @param dst Destination 
 * @param data Data to put
 * @len length of data (iini bytes)
 * @return The number of bytes put
 */
int lw_put_data(uint8_t*dst,const uint8_t*data,uint16_t len)
{
	memcpy(dst,data,len); 
	return len;
}


/**
 * Put a #bstr16_t to an output buffer
 * @param dst Destination 
 * @param b bstr16 to put
 * @return The number of bytes put
 */
int lw_put_bstr16(uint8_t * dst, const bstr16_t b){
	lw_put_data(dst,bstr16_data(b),bstr16_len(b));
	return bstr16_len(b);
}



int lw_put_elem_hdr(uint8_t *dst,uint8_t type,uint16_t len)
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
int lw_put_vendor(uint8_t * dst, uint32_t vendorid,
					 uint16_t elemid, uint16_t len)
{
	lw_set_dword(dst + 0, vendorid);
	lw_set_word(dst + 4, elemid);
	return 6;
}




