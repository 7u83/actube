



#include "cw/action.h"
#include "cw/cw.h"
#include "cw/mavl.h"



int cisco_out_ac_ipv4_list(struct conn *conn, struct cw_action_out *a, uint8_t * dst)
{
/*	mbag_t ips = conn->local;
	MAVLITER_DEFINE (it,ips);

	mavliter_foreach(&it){

	}
*/

	struct sockaddr sa;
	sock_strtoaddr( "192.168.0.11", &sa);
	cw_put_sockaddr(dst+10,(struct sockaddr_storage*)&sa);
	int l = 4;
	return l + cw_put_elem_vendor_hdr(dst, a->vendor_id, a->elem_id, l);


}
