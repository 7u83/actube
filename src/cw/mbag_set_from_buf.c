#include "mbag.h"
#include "item.h"
#include "cw.h"

int mbag_set_from_buf(mbag_t dst, cw_itemdef_t * idef, const char *item_id, uint8_t *data, int len)
{	

/*	if (!idef) {
		cw_log(LOG_ERR,"No itemdef found for %s",item_id);
		return 0;
	}


	if (idef->type->mbag_set){
		printf("Found a set function for %s\n",idef->type->name);
		idef->type->mbag_set(idef,item_id, 
		exit(0);
	}
*/

	if (idef->type == MBAG_BYTE) {
		mbag_set_byte(dst, item_id, *data);
		return 1;
	}
	if (idef->type == MBAG_WORD) {
		mbag_set_word(dst, item_id, cw_get_word(data));
		return 1;
	}
	if (idef->type == MBAG_DWORD) {
		mbag_set_dword(dst, item_id, cw_get_dword(data));
		return 1;
	}
	if (idef->type == MBAG_STR) {
		mbag_set_strn(dst, item_id, (char *) data, len);
		return 1;
	}
	if (idef->type == MBAG_BSTR) {
		mbag_set_bstrn(dst, item_id, data, len);
		return 1;
	}

	if (idef->type == MBAG_BSTR16) {
		mbag_set_bstr16n(dst, item_id, data, len);
		return 1;
	}

/*	if (idef->type == MBAG_DATA) {
		mbag_set_data(dst, item_id, data, len);
		return 1;
	}
*/
	if (idef->type == MBAG_VENDORSTR) {
		mbag_set_bstrv(dst, item_id,
				   cw_get_dword(data), data + 4, len - 4);
		return 1;
	}



//	printf("Idef: %s\n",idef->type->name);



/*	cw_log(LOG_ERR,
	       "Can't handle item type %d in definition for incomming msg %d (%s) - %d, cw_in_generic.",
	       idef->type, a->msg_id, cw_strmsg(a->msg_id), a->elem_id);
	return 0;
*/
	return 0;

}


