
#include "lw.h"

/**
 * Put a string to an output buffer
 * @param dst Output buffer
 * @param str zero-terminated string to put
 * @return number of bytes put
 */
int lw_put_str(uint8_t*dst,const uint8_t *str) {
	return lw_put_data(dst,str,strlen((char*)str));
}


