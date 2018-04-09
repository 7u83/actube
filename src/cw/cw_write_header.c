
#include "msgset.h"
#include "cw.h"

int cw_write_header(struct cw_ElemHandler * handler, uint8_t * dst, int len)
{
	if (handler->vendor)
		return len + cw_put_elem_vendor_hdr(dst, handler->vendor, handler->id, len);

	return  len + cw_put_elem_hdr(dst, handler->id, len);
}

