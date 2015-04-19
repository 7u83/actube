#include "log.h"
#include "conn.h"

#include "mbag.h"
#include "capwap_items.h"
#include "capwap.h"


int cw_out_wtp_board_data(struct conn *conn, struct cw_action_out *a, uint8_t * dst)
{
	mbag_t board_data =
	    mbag_get_avltree(conn->local, CW_ITEM_WTP_BOARD_DATA);
	if (!board_data) {
		cw_log(LOG_ERR, "Error: Can't send WTP Board Data element. WTP Board Data is not defined.");
		return 0;
	}

	struct mbag_item *i;
	i = mbag_get(board_data, CW_ITEM_WTP_BOARD_VENDOR);
	if (!i) {
		cw_log(LOG_ERR,
		       "Error: Can't send WTP Board Data element - no vendor defined");
		return 0;
	}

	uint8_t *d = dst + 4;
	d += cw_put_dword(d, i->dword);

	i = mbag_get(board_data, CW_ITEM_WTP_BOARD_MODELNO);
	if (i) {
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
