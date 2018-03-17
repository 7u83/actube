#include "cw/mbag.h"
#include "cw/log.h"
#include "cw/conn.h"

#include "cw/mbag.h"
#include "cw/capwap_items.h"
#include "cw/capwap.h"


#include "cw/cw.h"
#include "cw/ktv.h"
#include "cw/keys.h"



static int put_ac_status(mavl_t global, mavl_t local, uint8_t *dst, const char * parent_key){

	uint8_t *d = dst;
	uint8_t security;
	
	char key[CW_KTV_MAX_KEY_LEN];
	

	d += cw_put_word(d,cw_ktv_get_word(global,"ac/ac-descriptor/stations",0));
	d += cw_put_word(d,cw_ktv_get_word(global,"ac/ac-descriptor/station-limit",0));
	d += cw_put_word(d,cw_ktv_get_word(global,"ac/ac-descriptor/active-wtps",0));
	d += cw_put_word(d,cw_ktv_get_word(global,"ac/ac-descriptor/max-wtps",0));

	security = 0;
	if (cw_ktv_get(local,"dtls-cert-file",CW_TYPE_BSTR16))
		security |= CAPWAP_FLAG_AC_SECURITY_X;
		
	if (cw_ktv_get(local,"dtls-psk",CW_TYPE_BSTR16))
		security |= CAPWAP_FLAG_AC_SECURITY_S;

	if (security == 0){
		cw_log(LOG_WARNING,"Attention: no AC security selected");
	}
	d += cw_put_byte(dst,security);
	
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
	int len;
	uint8_t *d = dst+4;
	char key[CW_KTV_MAX_KEY_LEN];

	d+=put_ac_status(params->conn->local_cfg,
				params->conn->global_cfg,
				d, eh->key);

	sprintf(key,"%s/%s",eh->key,CW_SKEY_HARDWARE);
	d+=cw_write_descriptor_subelem (d, params->conn->local_cfg,
                                 CW_SUBELEM_AC_HARDWARE_VERSION, key);
 
	sprintf(key,"%s/%s",eh->key,CW_SKEY_SOFTWARE);
	d+=cw_write_descriptor_subelem (d, params->conn->local_cfg,
                                 CW_SUBELEM_AC_HARDWARE_VERSION, key);

	len = d-dst-4;

	return len + cw_put_elem_hdr(dst,eh->id,len);

}
