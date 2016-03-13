#include "log.h"
#include "conn.h"

#include "mbag.h"
#include "capwap_items.h"
#include "cw.h"


int cw_get_out_item(struct conn * conn, struct cw_action_out *a, void *dst, mbagtype_t type)
{
	if (!a->get)
		return 0;

	/* Get the item */
	struct mbag_item *item = a->get(conn, a);
	if (!item)
		return 0;



	if (item->type != type){
		return 0;
	}

	return cw_put_mbag_item(dst,item);
}

int cw_out_wtp_board_data(struct conn *conn, struct cw_action_out *a, uint8_t * dst)
{

	mbag_t board_data;
      	 
//	    mbag_get_mbag(conn->config, CW_ITEM_WTP_BOARD_DATA,NULL);

 	if (!cw_get_out_item(conn,a,&board_data,MBAG_MBAG)){
		cw_log(LOG_ERROR,"Can't get item '%s' for msg %d, elem %d",a->item_id,a->msg_id,a->elem_id);
		return 0;
	}

/*	if (!board_data) {
		cw_log(LOG_ERR, "Error: Can't send WTP Board Data element. \"%s\" is undefined.",CW_ITEM_WTP_BOARD_DATA);
		return 0;
	}
*/
	struct mbag_item *i;
	i = mbag_get(board_data, CW_ITEM_WTP_BOARD_VENDOR);
	if (!i) {
		cw_log(LOG_ERR,
		       "Error: Can't send WTP Board Data element - no vendor defined");
		return 0;
	}

	uint8_t *d = dst + 4;
	d += cw_put_dword(d, i->dword);

char buf[90];

	i = mbag_get(board_data, CW_ITEM_WTP_BOARD_MODELNO);
	if (i) {

printf("We have got an i %p\n",i);
printf("len: %d\n",bstr16_len(i->data));
memcpy(buf,bstr16_data(i->data),bstr16_len(i->data));
buf[bstr16_len(i->data)]=0;
printf("Val: %s\n",buf);

		d += cw_put_word(d, CW_BOARDDATA_MODELNO);
		d += cw_put_word(d, bstr16_len(i->data));
		d += cw_put_data(d, bstr16_data(i->data), bstr16_len(i->data));
	} else {
		cw_log(LOG_ERR,
		       "Error: Can't set sub-element \"WTP Model Number\" in WTP Board Data.");
	}

	i = mbag_get(board_data, CW_ITEM_WTP_BOARD_SERIALNO);
	if (i) {
		d += cw_put_word(d, CW_BOARDDATA_SERIALNO);
		d += cw_put_word(d, bstr16_len(i->data));
		d += cw_put_data(d, bstr16_data(i->data), bstr16_len(i->data));
	}else {
		cw_log(LOG_ERR,
		       "Error: Can't set sub-element \"WTP Serial Number\" in WTP Board Data.");
	}


	i = mbag_get(board_data, CW_ITEM_WTP_BOARD_ID);
	if (i) {
		d += cw_put_word(d, CW_BOARDDATA_BOARDID);
		d += cw_put_word(d, bstr16_len(i->data));
		d += cw_put_data(d, bstr16_data(i->data), bstr16_len(i->data));
	}

	i = mbag_get(board_data, CW_ITEM_WTP_BOARD_REVISION);
	if (i) {
		d += cw_put_word(d, CW_BOARDDATA_REVISION);
		d += cw_put_word(d, bstr16_len(i->data));
		d += cw_put_data(d, bstr16_data(i->data), bstr16_len(i->data));
	}

	i = mbag_get(board_data, CW_ITEM_WTP_BOARD_MACADDRESS);
	if (i) {
		d += cw_put_word(d, CW_BOARDDATA_MACADDRESS);
		d += cw_put_word(d, bstr_len(i->data));
		d += cw_put_data(d, bstr_data(i->data), bstr_len(i->data));
	}

	int l = d - dst-4;
	return l + cw_put_elem_hdr(dst, CW_ELEM_WTP_BOARD_DATA, l );

}
