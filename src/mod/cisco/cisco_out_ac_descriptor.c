
#include "cw/log.h"
#include "cw/dbg.h"
#include "cw/conn.h"



#include "cw/capwap.h"
#include "cw/cw.h"
#include "cw/val.h"
#include "cw/keys.h"
#include "cw/cfg.h"


static int put_ac_status(cw_Cfg_t * cfg1, cw_Cfg_t * cfg2, uint8_t *dst, const char * parent_key){

	uint8_t *d = dst;
//	uint8_t security;
	
	char key[CW_CFG_MAX_KEY_LEN];
	
	d += cw_put_word(d,cw_dget(cw_cfg_get_word,cfg1,cfg2,"ac-descriptor/stations",0));
	d += cw_put_word(d,cw_dget(cw_cfg_get_word,cfg1,cfg2,"ac-descriptor/station-limit",0));
	d += cw_put_word(d,cw_dget(cw_cfg_get_word,cfg1,cfg2,"ac-descriptor/active-wtps",0));
	d += cw_put_word(d,cw_dget(cw_cfg_get_word,cfg1,cfg2,"ac-descriptor/max-wtps",0));
	
	d += cw_put_byte(d,cw_dget(cw_cfg_get_byte,cfg1,cfg2,"ac-descriptor/security",0));
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
	sprintf(key,"%s/%s",parent_key,"ac-descriptor/r-mac-field");
	d += cw_put_byte(d,cw_dget(cw_cfg_get_byte,cfg1,cfg2,"ac-descriptor/r-mac-field",0));
		/*d += cw_put_byte(d,3);*/
	
	d += cw_put_byte(d,0);


	sprintf(key,"%s/%s",parent_key,CW_SKEY_DTLS_POLICY);
	d += cw_put_byte(d,cw_dget(cw_cfg_get_byte,cfg1,cfg2,"ac-descriptor/dtls-policy",0));

	return d - dst;
}


int cisco_out_ac_descriptor(struct cw_ElemHandler * eh, 
		struct cw_ElemHandlerParams * params, uint8_t * dst)
{
	int len,l;
	uint8_t *d = dst+4;
	char key[CW_CFG_MAX_KEY_LEN];

	d+=put_ac_status(params->conn->local_cfg,
				params->conn->global_cfg,
				d, eh->key);

	/* it is important to send software version first, 
	 * because APs don't check the type */
	sprintf(key,"%s/%s",eh->key,CW_SKEY_SOFTWARE);
	d+=cw_write_descriptor_subelem (d, params->cfg_list,
                                 1, key);

	sprintf(key,"%s/%s",eh->key,CW_SKEY_HARDWARE);
	d+=cw_write_descriptor_subelem (d, params->cfg_list,
                                 0, key);
 


	len = d-dst-4;

	l = len + cw_put_elem_hdr(dst,eh->id,len);
	cw_dbg_elem(DBG_ELEM_OUT,NULL,params->msgdata->type,eh,dst,l);

	return l;

}
