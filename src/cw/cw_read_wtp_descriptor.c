#include "cw.h"

#include "capwap.h"
#include "capwap_items.h"
#include "dbg.h"
#include "cw_types.h"


static struct cw_DescriptorSubelemDef allowed_default[] = {
	{0,CW_SUBELEM_WTP_HARDWARE_VERSION, "hardware_version", 1024,1},
	{0,CW_SUBELEM_WTP_SOFTWARE_VERSION, "software_version", 1024,1},
	{0,CW_SUBELEM_WTP_BOOTLOADER_VERSION, "bootloader_version", 1024,1},
	{0,CW_SUBELEM_WTP_OTHERSOFTWARE_VERSION, "other_software_version", 1024,0},
	{0,0, NULL, 0,0}
};


int cw_read_wtp_descriptor(mavl_t cfg, struct conn *conn,
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

	/* Get number of encryption elements */
	ncrypt = cw_get_byte(data + 2);
	if (ncrypt == 0) {
		if (conn->strict_capwap) {
			cw_dbg(DBG_ELEM_ERR,
			       "Bad WTP Descriptor, number of encryption elements is 0.");
			return 0;
		}
		cw_dbg(DBG_RFC,
		       "Non standard conform WTP Descriptor, number of encryptoin elements is 0.");
	}

	pos = 3;
	for (i = 0; i < ncrypt; i++) {
/*		// It's a dummy for now */
		pos += 3;
	}


	if (!allowed) {
		allowed=allowed_default;
	}
printf("call read subelems\n");
	return cw_read_descriptor_subelems(cfg, data + pos, len - pos, allowed);
}
