
#include "cw/log.h"
#include "cw/cw.h"
#include "cw/val.h"
#include "cw/keys.h"
#include "cw/dbg.h"


static int cw_put_encryption_subelems(uint8_t *dst,int capwap_mode)
{
	int n=2;
	int i;
	
	dst+=cw_put_byte(dst,n);

	
	for (i=0; i<n; i++){
		dst+=cw_put_byte(dst,0);
		dst+=cw_put_byte(dst,0);
		dst+=cw_put_byte(dst,0);
	}

	return 3*n+1;
}


int capwap_out_wtp_descriptor(struct cw_ElemHandler * eh, 
		struct cw_ElemHandlerParams * params, uint8_t * dst)
{
	char key[CW_CFG_MAX_KEY_LEN];
	int len,l;
/*	// XXX Dummy WTP Descriptor Header */
	uint8_t *d; 
	int val;
	
	d = dst+4;

	sprintf(key,"%s/%s",eh->key,CW_SKEY_MAX_RADIOS);
	val = cw_cfg_get_byte_l(params->cfg_list,key, 0);
	d+=cw_put_byte(d,val);
	if (val<=0){
		cw_dbg(DBG_WARN,"Cannot value for %s, setting to 0", CW_SKEY_MAX_RADIOS);
		d+=cw_put_byte(d,0);
	}
		
	sprintf(key,"%s/%s",eh->key,CW_SKEY_RADIOS_IN_USE);
	val = cw_cfg_get_byte_l(params->cfg_list,key, 0);
		d+=cw_put_byte(d,val);	/*radios in use*/

/* 	d+=cw_put_encryption_capabilities_7(d,1); */
/*	d+=cw_put_encryption_subelems(d,params->conn->capwap_mode);*/
	d+=cw_put_encryption_subelems(d,0);


/*cw_ktv_dump(params->conn->local_cfg,DBG_INFO,"*** ktv dump ***","","*** end of dump ***");*/

	/* hardware version sub element */
	sprintf(key,"%s/%s",eh->key,CW_SKEY_HARDWARE);
	d+=cw_write_descriptor_subelem (d, params->cfg,
                                 CW_SUBELEM_WTP_HARDWARE_VERSION, key);
				 
	/* software version sub element */
	sprintf(key,"%s/%s",eh->key,CW_SKEY_SOFTWARE);
	d+=cw_write_descriptor_subelem (d, params->cfg,
                                 CW_SUBELEM_WTP_SOFTWARE_VERSION, key);

	/* bootloader version sub element */
	sprintf(key,"%s/%s",eh->key,CW_SKEY_BOOTLOADER);
	d+=cw_write_descriptor_subelem (d, params->cfg,
                                 CW_SUBELEM_WTP_BOOTLOADER_VERSION, key);

	len = d-dst-4;
	l = len + cw_put_elem_hdr(dst,eh->id,len);
	
//	cw_dbg_elem(DBG_ELEM_OUT,NULL,params->msgdata->type,eh,dst,l);
	return l;
}
