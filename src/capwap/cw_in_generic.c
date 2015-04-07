
#include "action.h"
#include "cw_log.h"
#include "itemstore.h"
#include "capwap.h"



int cw_in_generic(struct conn *conn,struct cw_action_in * a,uint8_t *data,int len)
{
	if (len<a->min_len) {
		cw_dbg(DBG_ELEM_ERR,"Message element too short, %d < %d", len,a->min_len);
		return 0;
	}
	if (len>a->max_len) {
		cw_dbg(DBG_ELEM_ERR,"Message element too big, %d > %d", len,a->max_len);
		return 0;
	}
	

	cw_itemstore_set(conn->remote,a->item_id,a->itemtype,data,len);

	return 0;
}
