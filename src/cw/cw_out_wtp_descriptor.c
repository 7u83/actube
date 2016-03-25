#include "log.h"
#include "conn.h"

//#include "itemstore.h"
#include "capwap_items.h"
#include "capwap.h"

#include "cw.h"


static int cw_put_encryption_subelems(uint8_t *dst,int capwap_mode)
{
	if (capwap_mode==CW_MODE_CISCO){
		cw_put_word(dst,0x01);
		return 2;
	}

	int n=2;
	
	dst+=cw_put_byte(dst,n);	

	int i;
	for (i=0; i<n; i++){
		dst+=cw_put_byte(dst,0);
		dst+=cw_put_byte(dst,0);
		dst+=cw_put_byte(dst,0);
	}

	return 3*n+1;
}



int cw_out_wtp_descriptor(struct conn *conn, struct cw_action_out *a, uint8_t * dst)
{

	mbag_t mbag = conn->config;

	// XXX Dummy WTP Descriptor Header
	uint8_t *d = dst+4;

	d+=cw_put_byte(d,conn->radios->count);	//max radios
	d+=cw_put_byte(d,2);	//radios in use

	d+=cw_put_encryption_subelems(d,conn->capwap_mode);

	mbag_item_t * i;
	i = mbag_get(mbag,CW_ITEM_WTP_HARDWARE_VERSION);
	if ( i ) {	
	 	d += cw_put_version(d,CW_SUBELEM_WTP_HARDWARE_VERSION,i->data);
	}
	else {
		cw_log(LOG_ERR, "Can't send Hardware Version in WTP Descriptor, not set.");
	}

	i = mbag_get(mbag,CW_ITEM_WTP_SOFTWARE_VERSION);
	if ( i ) {	
	 	d += cw_put_version(d,CW_SUBELEM_WTP_SOFTWARE_VERSION,i->data);
	}
	else {
		cw_log(LOG_ERR, "Can't send Software Version in WTP descriptor, not set.");
	}

        i = mbag_get(mbag,CW_ITEM_WTP_BOOT_VERSION);
        if ( i ) {
                d += cw_put_version(d,CW_SUBELEM_WTP_BOOTLOADER_VERSION,i->data);
        }
        else {
                cw_log(LOG_INFO, "Can't send Boot Version in WTP descriptor, not set.");
        }

        i = mbag_get(mbag,CW_ITEM_WTP_OTHER_VERSION);
        if ( i ) {
                d += cw_put_version(d,CW_SUBELEM_WTP_OTHERSOFTWARE_VERSION,i->data);
        }
        else {
                cw_log(LOG_INFO, "Can't send Other Version in WTP descriptor, not set.");
        }

	int len = d-dst-4;
	return len + cw_put_elem_hdr(dst,a->elem_id,len);
}	
