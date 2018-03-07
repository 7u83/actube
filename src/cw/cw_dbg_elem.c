#include <stdint.h>

#include "dbg.h"
#include "format.h"

/**
 * print debug info for message elements
 */

void cw_dbg_elem_x(int level, struct conn *conn, int msg, int msgelem,
			 const uint8_t * msgbuf, int len)
{


	const char *elemname;
	char vendorname[256];
	char vendor_details[265];

	if (!cw_dbg_is_level(level))
		return;
		
	*vendor_details = 0;

	
/*	
/// TODO XXXX
*/

/*
	if (msgelem == CAPWAP_ELEM_VENDOR_SPECIFIC_PAYLOAD) {
		uint32_t vendor_id = ntohl(*((uint32_t *) msgbuf));
		int type = ntohs(*((uint16_t *) (msgbuf + 4)));
		cw_format_vendor(vendor_details, vendor_id, type, msgbuf);
//		sprintf(vendorname, "%s/%s/%s",
//			cw_strelemp(conn->actions, msgelem),
//			(char *) cw_strvendor(vendor_id), vendor_details);

		elemname = vendorname;

	} else {
		
//		elemname = cw_strelemp(conn->actions, msgelem);

		elemname=0;
	}
*/

/*&
	if (!cw_dbg_is_level(DBG_ELEM_DMP)) {
		cw_dbg(DBG_ELEM, "%d (%s), len=%d", msgelem, elemname, len);
	} else {
		char *dmp;
		dmp = cw_format_dump(msgbuf,len,NULL);

		cw_dbg(DBG_ELEM, "%d (%s), len=%d%s%s",
		       msgelem, elemname, len, get_dbg_color_ontext(DBG_ELEM_DMP), dmp);

		free(dmp);
	}
*/
  }