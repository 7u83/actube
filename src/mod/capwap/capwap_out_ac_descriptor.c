
#include "cw/log.h"
#include "cw/dbg.h"
#include "cw/conn.h"

#include "cw/capwap.h"
#include "cw/cw.h"
#include "cw/keys.h"

#include "mod_capwap.h"

int capwap_out_ac_descriptor(struct cw_ElemHandler * eh, 
		struct cw_ElemHandlerParams * params, uint8_t * dst)
{
	int len,l;
	uint8_t *d = dst+4;
	char key[CW_CFG_MAX_KEY_LEN];

	d+=cw_put_ac_status(d, params->cfg_list, eh->key);

	sprintf(key,"%s/%s",eh->key,CW_SKEY_HARDWARE);
	d+=cw_put_descriptor_subelem (d, params->cfg_list,
                                 CAPWAP_SUBELEM_AC_HARDWARE_VERSION, key);
 
	sprintf(key,"%s/%s",eh->key,CW_SKEY_SOFTWARE);
	d+=cw_put_descriptor_subelem (d, params->cfg_list,
                                 CAPWAP_SUBELEM_AC_SOFTWARE_VERSION, key);

	len = d-dst-4;

	l = len + cw_put_elem_hdr(dst,eh->id,len);
	cw_dbg_elem(DBG_ELEM_OUT,NULL,params->msgdata->type,eh,dst,l);
	return l;

}
