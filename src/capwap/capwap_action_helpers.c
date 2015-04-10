
#include "action.h"
#include "capwap.h"




int cw_in_set_state_none(struct conn *conn,struct cw_action_in * a,uint8_t *data,int len)
{
	conn->capwap_state=CW_STATE_NONE;
	return 1;
}


struct cw_item *cw_out_get_outgoing(struct conn *conn, struct cw_action_out *a)
{
	return cw_itemstore_get(conn->outgoing, a->item_id);
}

struct cw_item *cw_out_get_incomming(struct conn *conn, struct cw_action_out *a)
{
	return cw_itemstore_get(conn->incomming, a->item_id);
}

struct cw_item *cw_out_get_local(struct conn *conn, struct cw_action_out *a)
{
	return cw_itemstore_get(conn->local,a->item_id);
}



#include "capwap.h"
#include "capwap_items.h"

#include "log.h"
#include "rand.h"

struct cw_item * cw_out_get_session_id(struct conn *conn,struct cw_action_out * a)
{
	printf("Vendor ID: %d\n", a->vendor_id);
	uint8_t session_id[16];
	int l = cw_rand(session_id,16);
	if ( l!=16 ) {
		cw_log(LOG_ERR,"Can't init session ID.");
		return NULL;
	}
	

	return cw_itemstore_set_bstrn(conn->local,CW_ITEM_SESSION_ID,session_id,4);
}

