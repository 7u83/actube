#include "cw.h"

#include "capwap.h"
#include "capwap_items.h"
#include "dbg.h"
#include "ktv.h"
#include "keys.h"


static struct cw_DescriptorSubelemDef allowed_default[] = {
	{0,CW_SUBELEM_WTP_HARDWARE_VERSION, CW_SKEY_HARDWARE, 1024,1},
	{0,CW_SUBELEM_WTP_SOFTWARE_VERSION, CW_SKEY_SOFTWARE, 1024,1},
	{0,CW_SUBELEM_WTP_BOOTLOADER_VERSION, CW_SKEY_BOOTLOADER, 1024,1},
	{0,CW_SUBELEM_WTP_OTHERSOFTWARE_VERSION, CW_SKEY_OTHER_SOFTWARE, 1024,0},
	{0,0, NULL, 0,0}
};


int cw_read_wtp_descriptor(mavl_t cfg, struct conn *conn,
			   struct cw_ElemHandler *eh, uint8_t * data, int len,
			   struct cw_DescriptorSubelemDef *allowed)
{



	int ncrypt, pos,i;
	char key[64];

	/* Get number of encryption elements */
	ncrypt = cw_get_byte(data + 2);
	if (ncrypt == 0) {
		if (conn->strict_capwap) {
			cw_dbg(DBG_ELEM_ERR,
			       "Bad WTP Descriptor, number of encryption elements is 0.");
			return CAPWAP_RESULT_UNRECOGNIZED_MESSAGE_ELEMENT;
		}
		cw_dbg(DBG_RFC,
		       "Non standard conform WTP Descriptor, number of encryptoin elements is 0.");
	}

	sprintf(key,"%s/%s",eh->key, CW_SKEY_MAX_RADIOS);
	cw_ktv_add(cfg,key,CW_TYPE_BYTE,data,1);

	sprintf(key,"%s/%s",eh->key, CW_SKEY_RADIOS_IN_USE);
	cw_ktv_add(cfg,key,CW_TYPE_BYTE,data+1,1);



	pos = 3;


	for (i = 0; i < ncrypt; i++) {
		/* It's a dummy for now */
		pos += 3;
	}


	if (!allowed) {
		allowed=allowed_default;
	}

	return cw_read_descriptor_subelems(cfg,eh->key , data + pos, len - pos, allowed);
}
