
#include <time.h>
#include "capwap_cisco.h"

int cw_out_cisco_ap_timesync(struct conn *conn,struct cw_action_out * a,uint8_t *dst)
{
	int l = cw_put_cisco_ap_timesync(dst+10,time(NULL),0);
	return l + cw_put_elem_vendor_hdr(dst, a->vendor_id, a->elem_id, l);
}

