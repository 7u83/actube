
#include "action.h"
#include "capwap.h"




int cw_in_set_state_none(struct conn *conn,struct cw_action_in * a,uint8_t *data,int len)
{
	conn->capwap_state=CW_STATE_NONE;
	return 1;
}

struct cw_item *cw_out_get_local(struct conn *conn, struct cw_action_out *a)
{
	return cw_itemstore_get(conn->local, a->item_id);
}

struct cw_item *cw_out_get_remote(struct conn *conn, struct cw_action_out *a)
{
	return cw_itemstore_get(conn->local, a->item_id);
}


