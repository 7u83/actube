
#include "cw/log.h"
#include "cw/dbg.h"
#include "cw/conn.h"



#include "cw/capwap.h"
#include "cw/cw.h"
#include "cw/val.h"
#include "cw/keys.h"



static int put_ac_status(mavl_t global, mavl_t local, uint8_t *dst, const char * parent_key){

	stop();

	uint8_t *d = dst;
	
	char key[CW_KTV_MAX_KEY_LEN];
	

	d += cw_put_word(d,cw_ktv_get_word(global,"ac-descriptor/stations",0));
	d += cw_put_word(d,cw_ktv_get_word(global,"ac-descriptor/station-limit",0));
	d += cw_put_word(d,cw_ktv_get_word(global,"ac-descriptor/active-wtps",0));
	d += cw_put_word(d,cw_ktv_get_word(global,"ac-descriptor/max-wtps",0));
	
	d += cw_put_byte(d,cw_ktv_get_byte(global,"ac-descriptor/security",0));
/*	
	security = 0;
	if (cw_ktv_get(local,"dtls-cert-file",CW_TYPE_BSTR16))
		security |= CAPWAP_FLAG_AC_SECURITY_X;
		
	if (cw_ktv_get(local,"dtls-psk",CW_TYPE_BSTR16))
		security |= CAPWAP_FLAG_AC_SECURITY_S;

	if (security == 0){
		cw_log(LOG_WARNING,"No AC security selected");
	}
	d += cw_put_byte(dst,security);
*/	
	sprintf(key,"%s/%s",parent_key,CW_SKEY_RMAC_FIELD);
	d += cw_put_byte(d,cw_ktv_get_byte(local,key,0));
	
	d += cw_put_byte(d,0);


	sprintf(key,"%s/%s",parent_key,CW_SKEY_DTLS_POLICY);
	d += cw_put_byte(d,cw_ktv_get_byte(local,key,0));

	return d - dst;
}


int capwap_out_ac_descriptor(struct cw_ElemHandler * eh, 
		struct cw_ElemHandlerParams * params, uint8_t * dst)
{
	int len,l;
	uint8_t *d = dst+4;
	char key[CW_KTV_MAX_KEY_LEN];

	d+=put_ac_status(params->cfg,
				params->default_cfg,
				d, eh->key);

	sprintf(key,"%s/%s",eh->key,CW_SKEY_HARDWARE);
	d+=cw_write_descriptor_subelem (d, params->cfg,
                                 CAPWAP_SUBELEM_AC_HARDWARE_VERSION, key);
 
	sprintf(key,"%s/%s",eh->key,CW_SKEY_SOFTWARE);
	d+=cw_write_descriptor_subelem (d, params->cfg,
                                 CAPWAP_SUBELEM_AC_SOFTWARE_VERSION, key);

	len = d-dst-4;

	l = len + cw_put_elem_hdr(dst,eh->id,len);
	cw_dbg_elem(DBG_ELEM_OUT,NULL,params->msgdata->type,eh,dst,l);

	return l;

}
