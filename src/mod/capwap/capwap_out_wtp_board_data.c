
#include "cw/log.h"
#include "cw/ktv.h"
#include "cw/msgset.h"
#include "cw/keys.h"
#include "cw/cw.h"


static int write_boarddata_subelem(uint8_t * dst, mavl_t ktv, const char * parent_key, 
			const char *skey, int type){
			
	char key[256];
	cw_KTV_t * val;
	uint8_t *d;
	
	d=dst;
	
	sprintf(key,"%s/%s",parent_key,skey);
	
	val = cw_ktv_get(ktv,key,CW_TYPE_BSTR16);
	
	if (val == NULL) {
		cw_log(LOG_ERR,
		       "Error: Can't set sub-element %d from key %s in WTP Board Data. Key not found",type,key);
		return 0;
	}

	
	
	d += cw_put_word(d, type);
	d += cw_put_word(d, val->type->len(val));
	d += val->type->put(val,d);
	
	return d-dst;
}



int capwap_out_wtp_board_data(struct cw_ElemHandler * eh, 
		struct cw_ElemHandlerParams * params, uint8_t * dst)
{
	cw_KTV_t * val;
	mavl_t cfg;
	uint8_t * d;
	char key[256];
	int l;
	
	d=dst+4;
	
	cfg = params->conn->local_cfg;
	
	sprintf(key,"%s/%s",eh->key,CW_SKEY_VENDOR);
	val = cw_ktv_get(cfg,key,CW_TYPE_DWORD);
	
	if (val ==NULL) {
		cw_log(LOG_ERR,
		       "Error: Can't send WTP Board Data element - %s not defined.", key);
		return 0;
	}

	d += val->type->put(val,d);
	/*cw_ktv_add(cfg,vendor_key,CW_TYPE_DWORD,data,len);*/
	
	d+=write_boarddata_subelem(d,cfg,eh->key,CW_SKEY_MODELNO,CW_BOARDDATA_MODELNO);
	d+=write_boarddata_subelem(d,cfg,eh->key,CW_SKEY_SERIALNO,CW_BOARDDATA_SERIALNO);
	d+=write_boarddata_subelem(d,cfg,eh->key,"board-id",CW_BOARDDATA_BOARDID);
	d+=write_boarddata_subelem(d,cfg,eh->key,"board-revision",CW_BOARDDATA_REVISION);
	d+=write_boarddata_subelem(d,cfg,eh->key,"mac-address",CW_BOARDDATA_MACADDRESS);
	

	l = d - dst-4;
	return l + cw_put_elem_hdr(dst, CAPWAP_ELEM_WTP_BOARD_DATA, l );

}
