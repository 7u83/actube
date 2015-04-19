

#include "capwap.h"
#include "capwap_items.h"

#include "log.h"
#include "rand.h"

int cw_out_init_session_id(struct conn *conn,struct cw_action_out * a,uint8_t *dst)
{
	printf("Vendor ID: %d\n", a->vendor_id);
	uint8_t session_id[16];
	int l = cw_rand(session_id,16);
	if ( l!=16 ) {
		cw_log(LOG_ERR,"Can't init session ID.");
		return -1;
	}
	

	cw_itemstore_set_bstrn(conn->local,CW_ITEM_SESSION_ID,session_id,16);
	return 0;
		
}

