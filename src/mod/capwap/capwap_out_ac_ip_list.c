



#include "cw/action.h"
#include "cw/cw.h"
#include "cw/mavl.h"



int capwap_out_ac_ip_list(struct conn *conn, struct cw_action_out *a, uint8_t * dst)
{
/*	mbag_t ips = conn->local;
	MAVLITER_DEFINE (it,ips);

	mavliter_foreach(&it){

	}
*/
	return 0;

	struct sockaddr sa;
	sock_strtoaddr( "192.168.0.11", &sa);
	cw_put_sockaddr(dst+4,(struct sockaddr_storage*)&sa);
	return 4 + cw_put_elem_hdr(dst,CW_ELEM_AC_IPV4_LIST,4);
}
