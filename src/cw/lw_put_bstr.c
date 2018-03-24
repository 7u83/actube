#include "lw.h"

/**
 * Put a #bstr_t to an output buffer
 * @param dst Destination 
 * @param b bstr to put
 * @return The number of bytes put
 */
int lw_put_bstr(uint8_t * dst, const bstr_t b){
        lw_put_data(dst,bstr_data(b),bstr_len(b));
        return bstr_len(b);
}
