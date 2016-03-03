
#include "capwap.h"

int cw_out_ac_name(uint8_t * dst,cw_item *item)
{
	uint8_t *data = item->data;
	int len = cw_put_data(dst+4,data,strlen(data));
	return len + cw_put_elem_hdr(str,CW_ELEM_AC_NAME,len);
}
