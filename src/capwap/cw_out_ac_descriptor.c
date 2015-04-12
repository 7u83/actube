#include "log.h"
#include "conn.h"

#include "itemstore.h"
#include "capwap_items.h"
#include "capwap.h"


/*
int cw_put_subelem_version(uint8_t *dst,uint16_t subelem_id, uint32_t vendor_id,bstr16_t data)
{

	uint8_t *d=dst;
	d += cw_put_dword(d,vendor_id);
	d += cw_put_dword(d, (subelem_id<<16) | bstr16_len(data));
	d += cw_put_data(d,bstr16_data(data),bstr16_len(data));
	return d-dst;
}
*/


int cw_out_ac_descriptor(struct conn *conn,struct cw_action_out * a,uint8_t *dst) 
{

	uint8_t *d = dst+4;
	struct cw_item * i;
	i = cw_itemstore_get(conn->local,CW_ITEM_AC_STATUS);
	
	if (!i) {
		cw_log(LOG_ERR,"Can't send AC Descriptor, no AC Status Item found");
		return 0; 
	}

	d+=cw_put_ac_status(d ,(struct cw_ac_status*)(i->data));



	i = cw_itemstore_get(conn->local,CW_ITEM_AC_HARDWARE_VERSION);
	if ( i ) {	
	 	d += cw_put_version(d,CW_SUBELEM_AC_HARDWARE_VERSION,i->data);
	}
	else {
		cw_log(LOG_ERR, "Can't send hard version in AC descriptor, not set.");
	}

	
	i = cw_itemstore_get(conn->local,CW_ITEM_AC_SOFTWARE_VERSION);

	if ( i ) {	
	 	d += cw_put_version(d,CW_SUBELEM_AC_SOFTWARE_VERSION,i->data);
	}
	else {
		cw_log(LOG_ERR, "Can't send software version in AC descriptor, not set.");
	}

	int len = d-dst-4;
	
	return len + cw_put_elem_hdr(dst,a->elem_id,len);

}



