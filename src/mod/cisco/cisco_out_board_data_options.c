#include "cw/cw.h"


extern int cisco_out_board_data_options(struct conn *conn, struct cw_action_out *a,
				    uint8_t * dst)
{
	int l = cw_put_dword(dst+10,0x01010401);
	return l + cw_put_elem_vendor_hdr(dst, a->vendor_id, a->elem_id, l);
	
}


