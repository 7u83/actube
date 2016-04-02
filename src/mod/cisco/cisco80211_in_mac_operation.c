#include "cw/cw.h"
#include "cw/cw_80211.h"
#include "cw/dbg.h"

int cisco80211_in_mac_operation(struct conn *conn, struct cw_action_in *a, uint8_t * data,
		                         int len, struct sockaddr *from)
{
	int rid = cw_get_byte(data);
	mbag_t r = mbag_i_get_mbag(conn->radios,rid,NULL);
	if (!r){
		cw_dbg(DBG_ELEM_ERR,"Radio %d not defined. Can't set mac operation.");
		return 0;
	}

	return cw_read_80211_mac_operation(data+3,r);
}

