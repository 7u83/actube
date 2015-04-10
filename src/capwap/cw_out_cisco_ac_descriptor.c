#include "log.h"
#include "conn.h"

#include "itemstore.h"
#include "capwap_items.h"
#include "capwap.h"


int cw_out_cisco_ac_descriptor(struct conn *conn,struct cw_action_out * a,uint8_t *dst) 
{

	uint8_t *d = dst+4;
	struct cw_item * i;
	i = cw_itemstore_get(conn->outgoing,CW_ITEM_AC_STATUS);
	
	if (!i) {
		cw_log(LOG_ERR,"Can't send AC Descriptor, no AC Status Item found");
		return 0; 
	}

	d+=cw_put_ac_status(d ,(struct cw_ac_status*)(i->data));

	
	i = cw_itemstore_get(conn->outgoing,CW_ITEM_AC_SOFTWARE_VERSION);

	if ( i ) {	
	 	d += cw_put_version(d,1,i->data);
	}
	else {
		cw_dbg(DBG_ELEM_ERR, "Can't send software version in AC descriptor, not set.");
	}

	int len = d-dst-4;
	
	return len + cw_put_elem_hdr(dst,a->elem_id,len);

}



