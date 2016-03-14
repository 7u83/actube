#include "cw.h"

#include "capwap.h"
#include "capwap_items.h"

/**
 * Read WTP Descriptor in Cisco-Style (Draft 7)
 */
int cw_read_wtp_descriptor_7(mbag_t mbag, struct conn *conn,
			     struct cw_action_in *a, uint8_t * data, int len,
			     struct cw_descriptor_subelem_def *allowed)
{

	mbag_set_byte(mbag, CW_ITEM_WTP_MAX_RADIOS, cw_get_byte(data));
	mbag_set_byte(mbag, CW_ITEM_WTP_RADIOS_IN_USE, cw_get_byte(data + 1));


	int pos = 2;

	/* Encryption element, for now dumy XXX */
	//cw_get_word(data + pos + 2);
	pos += 2;


	static struct cw_descriptor_subelem_def allowed_default[] = {
		{-1,CW_SUBELEM_WTP_HARDWARE_VERSION, CW_ITEM_WTP_HARDWARE_VERSION, 1024,0},
		{-1,CW_SUBELEM_WTP_SOFTWARE_VERSION, CW_ITEM_WTP_SOFTWARE_VERSION, 1024.0},
		{-1,CW_SUBELEM_WTP_BOOTLOADER_VERSION, CW_ITEM_WTP_SOFTWARE_VERSION, 1024,0},
		{-1,CW_SUBELEM_WTP_OTHERSOFTWARE_VERSION, CW_ITEM_WTP_SOFTWARE_VERSION, 1024,0},
		{0,0, NULL, 0,0}
	};

	if (!allowed)
		allowed=allowed_default;

	return cw_read_descriptor_subelems(mbag, data + pos, len - pos, allowed);
}
