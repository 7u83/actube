#include "cw.h"

#include "dbg.h"
#include "ktv.h"

static struct cw_DescriptorSubelemDef allowed_default[] = {
	{0,CW_SUBELEM_AC_HARDWARE_VERSION, "ac_hardware_versision", 1024,1},
	{0,CW_SUBELEM_AC_SOFTWARE_VERSION, "ac_software_version", 1024,1},
	{0,0, NULL,0, 0}
};


int cw_read_ac_descriptor(mavl_t store, uint8_t *data, int len, 
		struct cw_DescriptorSubelemDef *allowed)
{
	char key[CW_KTV_MAX_KEY_LEN];
	cw_KTV_t * stations, *limit;

	stations = cw_ktv_add(store,"ac-descriptor/stations",CW_TYPE_WORD,data,4);

/*	status->stations = cw_get_word(data);
	status->limit = cw_get_word(data + 2);
	status->active_wtps = cw_get_word(data + 4);
	status->max_wtps = cw_get_word(data + 6);
	status->security = cw_get_byte(data + 8);
	status->rmac_field = cw_get_byte(data + 9);
	status->dtls_policy = cw_get_byte(data + 11);

	cw_dbg(DBG_SUBELEM,
	       "AC Descriptor: WTPs:%d/%d, Stations:%d/%d, Security:%d, Rmac:%d, DTLS-Policy:%d",
	       status->active_wtps, status->max_wtps, status->stations, status->limit,
	       status->security, status->rmac_field, status->dtls_policy);
*/
/*	mbag_set_ptr(store, CW_ITEM_AC_STATUS, status);*/


	if (!allowed)
		allowed=allowed_default;

	return cw_read_descriptor_subelems(store, "key", data + 12, len - 12, allowed);
	
}


