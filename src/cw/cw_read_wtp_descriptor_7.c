#include "cw.h"

#include "capwap.h"
#include "capwap_items.h"
#include "cw_types.h"

/**
 * Read WTP Descriptor in Cisco-Style (Draft 7)
 */
int cw_read_wtp_descriptor_7(mavl_t cfg, struct conn *conn,
			     struct cw_ElemHandler *eh, uint8_t * data, int len,
			     struct cw_DescriptorSubelemDef *allowed)
{
	int ncrypt, pos,i;
	mavldata_t md;
	char key[64];

	sprintf(key,"%s/%s",eh->key, "max_radios");
	md.kv.key=strdup(key);
	mavl_replace (cfg, cw_type_byte.get(&md,data,1));

	sprintf(key,"%s/%s",eh->key, "radios_in_use");
	md.kv.key=strdup(key);
	mavl_replace (cfg, cw_type_byte.get(&md,data+1,1));

/*	mbag_set_byte(mbag, CW_ITEM_WTP_MAX_RADIOS, cw_get_byte(data));
	mbag_set_byte(mbag, CW_ITEM_WTP_RADIOS_IN_USE, cw_get_byte(data + 1));
*/

	pos = 2;

	/* Encryption element, for now dumy XXX */
	//cw_get_word(data + pos + 2);
	pos += 2;


	static struct cw_DescriptorSubelemDef allowed_default[] = {
		{-1,CW_SUBELEM_WTP_HARDWARE_VERSION, CW_ITEM_WTP_HARDWARE_VERSION, 1024,0},
		{-1,CW_SUBELEM_WTP_SOFTWARE_VERSION, CW_ITEM_WTP_SOFTWARE_VERSION, 1024.0},
		{-1,CW_SUBELEM_WTP_BOOTLOADER_VERSION, CW_ITEM_WTP_SOFTWARE_VERSION, 1024,0},
		{-1,CW_SUBELEM_WTP_OTHERSOFTWARE_VERSION, CW_ITEM_WTP_SOFTWARE_VERSION, 1024,0},
		{0,0, NULL, 0,0}
	};

	if (!allowed)
		allowed=allowed_default;

	return cw_read_descriptor_subelems(cfg, data + pos, len - pos, allowed);
}
