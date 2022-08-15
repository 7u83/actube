
#include "cw/conn.h"
#include "cw/log.h"
#include "cw/val.h"
#include "cw/msgset.h"
#include "cw/keys.h"
#include "cw/cw.h"
#include "cw/dbg.h"

static int write_boarddata_subelem(uint8_t * dst, cw_Cfg_t ** cfg, const char * parent_key, 
			const char *skey, int type, void *param){
			
	char key[CW_CFG_MAX_KEY_LEN];
	int len;
	
	sprintf(key,"%s/%s",parent_key,skey);
	
	//val = cw_ktv_get(ktv,key,CW_TYPE_BSTR16);
	len = cw_generic_write_l(cfg, CW_TYPE_BSTR16,key,
			dst+4, param);
	
	if (len==-1) {
		cw_log(LOG_ERR,
		       "Creating WTP Board Data sub-element %d. Key not '%s' found",type,key);
		return 0;
	}

	
	
	cw_set_word(dst, (uint16_t)type);
	cw_set_word(dst+2,(uint16_t)len);
	return len+4;
}



int capwap_out_wtp_board_data(struct cw_ElemHandler * eh, 
		struct cw_ElemHandlerParams * params, uint8_t * dst)
{
	int rc;
	uint8_t * d;
	char key[256];
	int l;
	
	d=dst+4;
	
	sprintf(key,"%s/%s",eh->key,CW_SKEY_VENDOR);
	rc = cw_generic_write_l(params->cfg_list, CW_TYPE_DWORD,key,
			d, eh->param);

	
	if (rc==-1) {
		cw_log(LOG_ERR,
		       "Creating WTP Board Data element. Key '%s' not found.", key);
		return 0;
	}

	d += rc;
	
	d+=write_boarddata_subelem(d,params->cfg_list,eh->key,"model-no",CW_BOARDDATA_MODELNO,eh->param);
	d+=write_boarddata_subelem(d,params->cfg_list,eh->key,"serial-no",CW_BOARDDATA_SERIALNO,eh->param);
	d+=write_boarddata_subelem(d,params->cfg_list,eh->key,"board-id",CW_BOARDDATA_BOARDID,eh->param);
	d+=write_boarddata_subelem(d,params->cfg_list,eh->key,"revision",CW_BOARDDATA_REVISION,eh->param);
	d+=write_boarddata_subelem(d,params->cfg_list,eh->key,"mac-address",CW_BOARDDATA_MACADDRESS,eh->param);
	

	l = d - dst-4;
	return l + cw_put_elem_hdr(dst, CAPWAP_ELEM_WTP_BOARD_DATA, l );

}
