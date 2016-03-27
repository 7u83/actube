
#include "cw/action.h"
#include "cw/capwap_items.h"
#include "cw/capwap.h"


struct mbag_item * capwap_out_get_idle_timeout(struct conn *conn,struct cw_action_out * a)
{
	struct mbag_item * m = mbag_get(conn->config,CW_ITEM_IDLE_TIMEOUT);
	if (m)
		return m;

	return mbag_set_dword(conn->config,CW_ITEM_IDLE_TIMEOUT,CAPWAP_IDLE_TIMEOUT);
}



struct mbag_item * capwap_out_get_capwap_timers(struct conn *conn,struct cw_action_out * a)
{
	struct mbag_item * m = mbag_get(conn->local,CW_ITEM_CAPWAP_TIMERS);
	if (m)
		return m;

	return mbag_set_word(conn->local,CW_ITEM_CAPWAP_TIMERS,CAPWAP_MAX_DISCOVERY_INTERVAL<<8 | CAPWAP_ECHO_INTERVAL);
}
