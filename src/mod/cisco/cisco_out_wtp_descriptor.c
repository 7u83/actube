#include "cw/log.h"
#include "cw/cw.h"
#include "cw/val.h"
#include "cw/keys.h"
#include "cw/dbg.h"

int cisco_out_wtp_descriptor(struct cw_ElemHandler * eh, 
		struct cw_ElemHandlerParams * params, uint8_t * dst)
{

	char key[CW_CFG_MAX_KEY_LEN];
	int len;
/*	// XXX Dummy WTP Descriptor Header */
	uint8_t *d; 
	int rc;
	
	d = dst+4;

	sprintf(key,"%s/%s",eh->key,"max-radios");
	rc = cw_generic_write_l(params->cfg_list, CW_TYPE_BYTE,key,
			d, eh->param);

	const char * 
		xxx = cw_cfg_get_l(params->cfg_list, key, NULL);
	cw_dbg(DBG_X,"What??? %s: %s",key,xxx);

//	val = cw_ktv_get(params->cfg,key, CW_TYPE_BYTE);
	if (rc==-1){
		cw_dbg(DBG_WARN,"Cannot get value for %s, setting to 0", key);
		d+=cw_put_byte(d,0);
	}
	else {
		int yyy = cw_get_byte(d);
	 	cw_dbg(DBG_X,"Verify: %d",yyy);
		d+=rc;
	}

		
	sprintf(key,"%s/%s",eh->key,CW_SKEY_RADIOS_IN_USE);

		xxx = cw_cfg_get_l(params->cfg_list, key, NULL);
	cw_dbg(DBG_X,"What??? %s: %s",key,xxx);
	
	rc = cw_generic_write_l(params->cfg_list, CW_TYPE_BYTE,key,
			d, eh->param);


//	val = cw_ktv_get(params->cfg,key, CW_TYPE_BYTE);
	if (rc == -1){
		cw_dbg(DBG_WARN,"Cannot get value for %s, setting to 0", key);
		d+=cw_put_byte(d,0);
	}
	else{
		d+=rc;	/*radios in use*/
	}

	d+=cw_put_encryption_capabilities_7(d,1);

cw_dbg(DBG_X,"befor subelem ^p",params->cfg_list);
	/* hardware version sub element */
	sprintf(key,"%s/%s",eh->key,CW_SKEY_HARDWARE);
	d+=cw_write_descriptor_subelem (d, params->cfg_list,
                                 CW_SUBELEM_WTP_HARDWARE_VERSION, key);
				 
	/* software version sub element */
	sprintf(key,"%s/%s",eh->key,CW_SKEY_SOFTWARE);
	d+=cw_write_descriptor_subelem (d, params->cfg_list,
                                 CW_SUBELEM_WTP_SOFTWARE_VERSION, key);

	/* bootloader version sub element */
	sprintf(key,"%s/%s",eh->key,CW_SKEY_BOOTLOADER);
	d+=cw_write_descriptor_subelem (d, params->cfg_list,
                                 CW_SUBELEM_WTP_BOOTLOADER_VERSION, key);

	len = d-dst-4;
	return len + cw_put_elem_hdr(dst,eh->id,len);
}
