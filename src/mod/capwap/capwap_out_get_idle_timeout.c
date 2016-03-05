
#include "cw/action.h"
#include "cw/capwap_items.h"


struct mbag_item * capwap_out_get_idle_timeout(struct conn *conn,struct cw_action_out * a)
{
	struct mbag_item * m = mbag_get(conn->config,CW_ITEM_IDLE_TIMEOUT);
	if (m)
		return m;

	return mbag_set_dword(conn->config,CW_ITEM_IDLE_TIMEOUT,300);
}
