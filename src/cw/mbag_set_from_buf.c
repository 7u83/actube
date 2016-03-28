#include "mbag.h"
#include "cw.h"

int mbag_set_from_buf(mbag_t dst, mbagtype_t type, const char *item_id, uint8_t *data, int len)
{	

	if (type == MBAG_BYTE) {
		mbag_set_byte(dst, item_id, *data);
		return 1;
	}
	if (type == MBAG_WORD) {
		mbag_set_word(dst, item_id, cw_get_word(data));
		return 1;
	}
	if (type == MBAG_DWORD) {
		mbag_set_dword(dst, item_id, cw_get_dword(data));
		return 1;
	}
	if (type == MBAG_STR) {
		mbag_set_strn(dst, item_id, (char *) data, len);
		return 1;
	}
	if (type == MBAG_BSTR) {
		mbag_set_bstrn(dst, item_id, data, len);
		return 1;
	}

	if (type == MBAG_BSTR16) {
		mbag_set_bstr16n(dst, item_id, data, len);
		return 1;
	}

	if (type == MBAG_VENDORSTR) {
		mbag_set_bstrv(dst, item_id,
				   cw_get_dword(data), data + 4, len - 4);
		return 1;
	}

	if (type->get){
		struct mbag_item * item = type->get(data,len);
		if (!item)
			return 0;
		item->id=item_id;
		mbag_set(dst,item);
		return 1;


	}

	return 0;

}


