
#include "cw/conn.h"
#include "cw/rand.h"
#include "cw/log.h"
#include "cw/capwap_items.h"


struct mbag_item * capwap_out_get_session_id(struct conn *conn,struct cw_action_out * a)
{
	uint8_t session_id[16];

	int slen=16;

	int l = cw_rand(session_id,slen);
	if ( l!=slen ) {
		cw_log(LOG_ERR,"Can't init session ID.");
		return NULL;
	}
	return mbag_set_bstrn(conn->local,CW_ITEM_SESSION_ID,session_id,slen);
}

