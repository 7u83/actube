
#include "action.h"
#include "cw_log.h"
#include "itemstore.h"



int cw_itemstore_set(cw_itemstore_t itemstore, uint32_t item_id, int item_type, uint8_t *data, int len)
{
	switch (item_type) {
		case CW_ITEMTYPE_BYTE:
			cw_itemstore_set_byte(itemstore,item_id,*data);	
			break;

		case CW_ITEMTYPE_STR:
			cw_itemstore_set_strn(itemstore,item_id,(char*)data,len);
			break;
		case CW_ITEMTYPE_BSTR:
			cw_itemstore_set_bstrn(itemstore,item_id,data,len);
			break;


	}
	return 0;
}

int cw_in_generic(struct conn *conn,struct cw_action_in * a,uint8_t *data,int len)
{
	if (len<a->min_len) {
		cw_dbg(DBG_ELEM_ERR,"Message element too short, %d < %d", len,a->min_len);
		return 0;
	}

	cw_itemstore_set(conn->remote,a->item_id,a->itemtype,data,len);
	
	return 0;
}
