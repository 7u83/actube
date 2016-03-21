#include "cw/log.h"
#include "cw/conn.h"

#include "cw/mbag.h"
#include "cw/capwap_items.h"
#include "cw/cw.h"


int cisco_out_ac_descriptor(struct conn *conn,struct cw_action_out * a,uint8_t *dst) 
{

	uint8_t *d = dst+4;
	struct mbag_item * i;
	i = mbag_get(conn->local,CW_ITEM_AC_STATUS);
	
	if (!i) {
		cw_log(LOG_ERR,"Can't send AC Descriptor, no AC Status Item found");
		return 0; 
	}

	d+=cw_put_ac_status(d ,(struct cw_ac_status*)(i->data),conn);

	
	/* Send back the same software as the WTP has,
	   otherwise the AP wants us to send an image */

	i = mbag_get(conn->incomming,CW_ITEM_WTP_SOFTWARE_VERSION);

	//i = mbag_get(conn->local,CW_ITEM_AC_SOFTWARE_VERSION);
	if ( i ) {	
	 	d += cw_put_version(d,1,i->data);
	}
	else {
		cw_log(LOG_ERR, "Can't set Cisco Software Version in AC descriptor, No value defined.");
	}

	i = mbag_get(conn->local,CW_ITEM_AC_HARDWARE_VERSION);
	if ( i ) {	
	 	d += cw_put_version(d,0,i->data);
	}
	else {
		cw_log(LOG_ERR, "Can't set Cisco Hardware Version in AC descriptor, No value defined.");
	}




	int len = d-dst-4;
	
	return len + cw_put_elem_hdr(dst,a->elem_id,len);

}



