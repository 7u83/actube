

#include "cw.h"
#include "capwap_items.h"

#include "dbg.h"
#include "log.h"

#include "acpriolist.h"



int cw_put_ac_name_with_priority(uint8_t *dst,cw_acprio_t * acprio)
{
	int len = strlen(acprio->name);
	cw_put_byte(dst,acprio->prio);
	cw_put_data(dst+1,(uint8_t*)acprio->name,len);
	return len+1;
	
}

int cw_out_ac_name_with_priority(struct conn *conn, struct cw_action_out *a, uint8_t * dst)
{
	cw_acpriolist_t prios = mbag_get_mavl(conn->config,CW_ITEM_AC_NAME_WITH_PRIORITY);
	if (!prios)
		return 0;
	
	uint8_t *d = dst;
	
	MAVLITER_DEFINE(it,prios);
	mavliter_foreach(&it){
		cw_acprio_t * ac = mavliter_get(&it);
		
		int l = cw_put_ac_name_with_priority(d+4,ac);
		d+=cw_put_elem_hdr(d,CW_ELEM_AC_NAME_WITH_PRIORITY,l)+l;		

	}

	return d-dst;
}

