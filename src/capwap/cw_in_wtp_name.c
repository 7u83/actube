#include "capwap.h"
#include "capwap_items.h"
#include "cw_log.h"
#include "itemstore.h"


int cw_in_wtp_name(struct conn *conn,struct cw_action * a,uint8_t *data,int len)
{
	if (len > 512){
		cw_dbg(DBG_CW_RFC, "WTP Name too long, must not exceed 512, len is %d",len);
	}
	cw_itemstore_set_strn(conn->itemstore,CW_ITEM_WTP_NAME,(const char*)data,len);
	return 1;
}
