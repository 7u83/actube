
#include "action.h"
#include "log.h"
#include "itemstore.h"
#include "capwap.h"



int cw_in_generic(struct conn *conn,struct cw_action_in * a,uint8_t *data,int len)
{
	if (len<a->min_len) {
		cw_dbg(DBG_ELEM_ERR,"Message element too short, %d < %d", len,a->min_len);
		return 0;
	}
	if (len>a->max_len) {
		cw_dbg(DBG_ELEM_ERR,"Message element too big, %d > %d", len,a->max_len);
		return 0;
	}
	
	
	cw_itemstore_t itemstore = conn->incomming;

        switch (a->itemtype) {
                case CW_ITEMTYPE_BYTE:
                        cw_itemstore_set_byte(itemstore,a->item_id,*data);
                        break;
                case CW_ITEMTYPE_WORD:
                        cw_itemstore_set_word(itemstore,a->item_id,cw_get_word(data));
                        break;
                case CW_ITEMTYPE_DWORD:
                        cw_itemstore_set_dword(itemstore,a->item_id,cw_get_dword(data));
                        break;
                case CW_ITEMTYPE_STR:
                        cw_itemstore_set_strn(itemstore,a->item_id,(char*)data,len);
                        break;
                case CW_ITEMTYPE_BSTR:
                        cw_itemstore_set_bstrn(itemstore,a->item_id,data,len);
                        break;
                case CW_ITEMTYPE_DATA:
                        cw_itemstore_set_data(itemstore,a->item_id,data,len);
                        break;
                case CW_ITEMTYPE_VENDORSTR:
                        cw_itemstore_set_vendorstr(itemstore,a->item_id,
                                cw_get_dword(data),data+4,len-4);
                

        }



//int src = 	cw_itemstore_set(conn->incomming,a->item_id,a->itemtype,data,len);

//printf("Cunint ic: %d %d\n",src,conn->incomming->count);

	return 0;
}
