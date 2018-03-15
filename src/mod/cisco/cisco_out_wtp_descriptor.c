#include "cw/log.h"
#include "cw/conn.h"
#include "cw/capwap_items.h"
#include "cw/cw.h"


#include "cisco.h"
#include "cw/ktv.h"
#include "cw/keys.h"
#include "cw/dbg.h"



int cisco_out_wtp_descriptor(struct cw_ElemHandler * eh, struct cw_ElemHandlerParams * params
			, uint8_t * dst)
{
	char key[256];
	int len;
/*	// XXX Dummy WTP Descriptor Header */
	uint8_t *d; 
	cw_KTV_t * val;
	
	d = dst+4;
	
	/*cw_ktv_init_byte(&val,8);*/
	sprintf(key,"%s/%s",eh->key,CW_SKEY_MAX_RADIOS);
	val = cw_ktv_get(params->conn->local_cfg,key, CW_TYPE_BYTE);
	if (val != NULL)
		d+=val->type->put(val,d);
	else{
		cw_dbg(DBG_WARN,"Cannot get value for %s, setting to 0", CW_SKEY_MAX_RADIOS);
		d+=cw_put_byte(d,0);
	}
		
		sprintf(key,"%s/%s",eh->key,CW_SKEY_RADIOS_IN_USE);
	val = cw_ktv_get(params->conn->local_cfg,key, CW_TYPE_BYTE);
	if (val != NULL){
		d+=val->type->put(val,d);
	}
	else{
		d+=cw_put_byte(d,0);	/*radios in use*/
	}


/*len = handler->type->put(elem,dst+start);*/



	d+=cw_put_encryption_capabilities_7(d,1);

	
/*        d += cw_put_dword(d, bstrv_get_vendor_id(v));
        d += cw_put_dword(d, (subelem_id << 16) | bstrv_len(v));
        d += cw_put_data(d, bstrv_data(v), bstrv_len(v));
*/

/*	sprintf(key,"%s/%s/%s",eh->key,CW_SKEY_HARDWARE,CW_SKEY_VENDOR);
	vendor = cw_ktv_get(params->conn->local_cfg,key,CW_TYPE_DWORD);
	sprintf(key,"%s/%s/%s",eh->key,CW_SKEY_HARDWARE,CW_SKEY_VENDOR);
	version = cw_ktv_get(params->conn->local_cfg,key,CW_TYPE_BSTR16);
	
	if (vendor == NULL){
		cw_log(LOG_ERR, "Can't send %s, not found.", key);
	}
*/

	/* hardware version sub element */
	sprintf(key,"%s/%s",eh->key,CW_SKEY_HARDWARE);
	d+=cw_write_descriptor_subelem (d, params->conn->local_cfg,
                                 CW_SUBELEM_WTP_HARDWARE_VERSION, key);
				 
	/* software version sub element */
	sprintf(key,"%s/%s",eh->key,CW_SKEY_SOFTWARE);
	d+=cw_write_descriptor_subelem (d, params->conn->local_cfg,
                                 CW_SUBELEM_WTP_SOFTWARE_VERSION, key);

	/* bootloader  version sub element */
	sprintf(key,"%s/%s",eh->key,CW_SKEY_BOOTLOADER);
	d+=cw_write_descriptor_subelem (d, params->conn->local_cfg,
                                 CW_SUBELEM_WTP_BOOTLOADER_VERSION, key);




/*	mbag_item_t * i;
	i = mbag_get(mbag,CW_ITEM_WTP_HARDWARE_VERSION);
	 */
/*	if ( i ) {	
	 	d += cw_put_version(d,CW_SUBELEM_WTP_HARDWARE_VERSION,i->u2.data);
	}
	else {
		cw_log(LOG_ERR, "Can't send Hardware Version in WTP Descriptor, not set.");
	}
*/

	/*i = mbag_get(mbag,CW_ITEM_WTP_SOFTWARE_VERSION);*/
/*
	if ( i ) {	
	 	d += cw_put_version(d,CW_SUBELEM_WTP_SOFTWARE_VERSION,i->u2.data);
	}
	else {
		cw_log(LOG_ERR, "Can't send Software Version in WTP descriptor, not set.");
	}

	i = mbag_get(mbag,CW_ITEM_WTP_BOOTLOADER_VERSION);
	if ( i ) {	
	 	d += cw_put_version(d,CW_SUBELEM_WTP_BOOTLOADER_VERSION,i->u2.data);
	}
	else {
		cw_log(LOG_ERR, "Can't send Bootloader Version in WTP descriptor, not set.");
	}
*/

	len = d-dst-4;
	return len + cw_put_elem_hdr(dst,eh->id,len);
}	
