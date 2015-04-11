#include "log.h"
#include "conn.h"

#include "itemstore.h"
#include "capwap_items.h"
#include "capwap.h"


int cw_out_wtp_board_data(struct conn *conn, struct cw_action_out *a, uint8_t * dst)
{
	cw_itemstore_t board_data =
	    cw_itemstore_get_avltree(conn->outgoing, CW_ITEM_WTP_BOARD_DATA);
	if (!board_data) {
		cw_log(LOG_ERR, "Error: Can't send WTP Board Data element - not defined");
		return 0;
	}

	struct cw_item *i;
	i = cw_itemstore_get(board_data, CW_ITEM_WTP_BOARD_VENDOR);
	if (!i) {
		cw_log(LOG_ERR,
		       "Error: Can't send WTP Board Data element - no vendor defined");
		return 0;
	}

	uint8_t *d = dst + 4;
	d += cw_put_dword(d, i->dword);

	i = cw_itemstore_get(board_data, CW_ITEM_WTP_BOARD_MODELNO);
	if (i) {
		d += cw_put_word(d, CW_BOARDDATA_MODELNO);
		d += cw_put_word(d, bstr16_len(i->data));
		d += cw_put_data(d, bstr16_data(i->data), bstr16_len(i->data));
	} else {
		cw_log(LOG_ERR,
		       "Error: Can't set sub-element \"Model No.\" in WTP Board Data.");
	}

	i = cw_itemstore_get(board_data, CW_ITEM_WTP_BOARD_SERIALNO);
	if (i) {
		d += cw_put_word(d, CW_BOARDDATA_SERIALNO);
		d += cw_put_word(d, bstr16_len(i->data));
		d += cw_put_data(d, bstr16_data(i->data), bstr16_len(i->data));
	}

	i = cw_itemstore_get(board_data, CW_ITEM_WTP_BOARD_ID);
	if (i) {
		d += cw_put_word(d, CW_BOARDDATA_BOARDID);
		d += cw_put_word(d, bstr16_len(i->data));
		d += cw_put_data(d, bstr16_data(i->data), bstr16_len(i->data));
	}

	i = cw_itemstore_get(board_data, CW_ITEM_WTP_BOARD_REVISION);
	if (i) {
		d += cw_put_word(d, CW_BOARDDATA_REVISION);
		d += cw_put_word(d, bstr16_len(i->data));
		d += cw_put_data(d, bstr16_data(i->data), bstr16_len(i->data));
	}

	i = cw_itemstore_get(board_data, CW_ITEM_WTP_BOARD_MACADDRESS);
	if (i) {
		d += cw_put_word(d, CW_BOARDDATA_MACADDRESS);
		d += cw_put_word(d, bstr_len(i->data));
		d += cw_put_data(d, bstr_data(i->data), bstr_len(i->data));
	}

	int l = d - dst-4;
	return l + cw_put_elem_hdr(dst, CW_ELEM_WTP_BOARD_DATA, l );

}
