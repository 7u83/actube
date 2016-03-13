
#include "action.h"
#include "capwap.h"



int cw_in_set_state_none(struct conn *conn,struct cw_action_in * a,uint8_t *data,int len)
{
	conn->capwap_state=CW_STATE_NONE;
	return 1;
}


struct mbag_item *cw_out_get_outgoing(struct conn *conn, struct cw_action_out *a)
{
	return mbag_get(conn->outgoing, a->item_id);
}

struct mbag_item *cw_out_get_incomming(struct conn *conn, struct cw_action_out *a)
{
	return mbag_get(conn->incomming, a->item_id);
}

struct mbag_item *cw_out_get_local(struct conn *conn, struct cw_action_out *a)
{
	return mbag_get(conn->local,a->item_id);
}

struct mbag_item *cw_out_get_config(struct conn *conn, struct cw_action_out *a)
{
//	printf("cw get config (%d %d %s) %p\n",a->msg_id, a->elem_id,a->item_id, conn->config);
	return mbag_get(conn->config,a->item_id);
}



#include "capwap.h"
#include "capwap_items.h"

#include "log.h"
#include "rand.h"

struct mbag_item * cw_out_get_session_id(struct conn *conn,struct cw_action_out * a)
{
	uint8_t session_id[16];

	int slen;
	if ( conn->capwap_mode == CW_MODE_CISCO){
		slen=2;
	}
	else
		slen=16;

	int l = cw_rand(session_id,slen);
	if ( l!=slen ) {
		cw_log(LOG_ERR,"Can't init session ID.");
		return NULL;
	}
	return mbag_set_bstrn(conn->local,CW_ITEM_SESSION_ID,session_id,slen);
}

