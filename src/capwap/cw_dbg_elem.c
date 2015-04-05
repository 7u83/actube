#include "cw_log.h"
#include "capwap.h"



/**
 * print debug info for message elements
 */

void cw_dbg_elem_(int msg, int msgelem, const uint8_t * msgbuf, int len)
{
	if (!cw_dbg_is_level(DBG_ELEM))
		return;

	const char *elemname;
	char vendorname[256];
	char vendor_details[265];
	*vendor_details = 0;

	if (msgelem == CW_ELEM_VENDOR_SPECIFIC_PAYLOAD) {
		uint32_t vendor_id = ntohl(*((uint32_t *) msgbuf));
		int type = ntohs(*((uint16_t *) (msgbuf + 4)));
		sprintf(vendorname, "%s/%s/%d",
			(char *) cw_strelem(msgelem),
			(char *) lw_vendor_id_to_str(vendor_id), type);
		elemname = vendorname;
		cw_format_vendor(vendor_details, vendor_id, type, msgbuf);

	} else {
		elemname = cw_strelem(msgelem);
	}


	if (!cw_dbg_is_level(DBG_ELEM_DMP))
		cw_dbg(DBG_ELEM,
		       "%s, CAWPAP element: %d (%s), len=%d%s",
		       cw_strmsg(msg), msgelem, elemname, len, vendor_details);

	else
		cw_dbg_dmp(DBG_ELEM, msgbuf, len,
			   "%s, CAPWAP element: %d (%s), len=%d%s\n\tDump ...",
			   cw_strmsg(msg), msgelem, elemname, len, vendor_details);
}


