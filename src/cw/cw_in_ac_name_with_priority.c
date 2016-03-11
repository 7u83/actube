
#include "action.h"
#include "dbg.h"
#include "log.h"
#include "mbag.h"
#include "cw.h"


#include "acpriolist.h"
#include "capwap_items.h"


int cw_in_ac_name_with_priority(struct conn *conn, struct cw_action_in *a, uint8_t * data, int len,
		  struct sockaddr *from)
{
	cw_acpriolist_t prios = mbag_get_mavl(conn->config,CW_ITEM_AC_NAME_WITH_PRIORITY);
	if (!prios)
		return 0;

	cw_acprio_t * prio = malloc(sizeof(cw_acprio_t));
	if (!prio)
		return 0;

	prio->prio=cw_get_byte(data);
	prio->name=strndup((char*)data+1,len-1);
	mavl_replace(prios,prio);	

	return 1;
}

