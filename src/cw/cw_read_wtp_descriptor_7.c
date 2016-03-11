#include "cw.h"

#include "capwap.h"
#include "capwap_items.h"

/**
 * Read WTP Descriptor in Cisco-Style (Draft 7)
 */
int cw_read_wtp_descriptor_7(mbag_t mbag, struct conn *conn,
			     struct cw_action_in *a, uint8_t * data, int len)
{

	mbag_set_byte(mbag, CW_ITEM_WTP_MAX_RADIOS, cw_get_byte(data));
	mbag_set_byte(mbag, CW_ITEM_WTP_RADIOS_IN_USE, cw_get_byte(data + 1));


	int pos = 2;

	/* Encryption element, for now dumy XXX */
	//cw_get_word(data + pos + 2);
	pos += 2;

	return cw_read_wtp_descriptor_versions(mbag, data + pos, len - pos);
}
