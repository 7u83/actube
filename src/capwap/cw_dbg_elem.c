

#include "log.h"
#include "capwap.h"
#include "capwap_cisco.h"
#include "dbg.h"



static int cw_format_vendor(char *dst, uint32_t vendor_id, int elem_id, const uint8_t * elem_data)
{
	switch (vendor_id) {
		case CW_VENDOR_ID_CISCO:
			{
				sprintf(dst, "%d - %s", elem_id,
					cw_cisco_id_to_str(elem_id));

				/* dive into LWAPP vendor specific decoding */
/*
				if (elem_id == CW_CISCO_SPAM_VENDOR_SPECIFIC) {
					uint32_t lw_elem_id = lw_get_word(elem_data + 4 + 6);
					char b[256];
					sprintf(b, "\n\t   LWAPP Cisco Specific: %d - %s",
						lw_elem_id, lw_cisco_id_to_str(lw_elem_id));
					strcat(dst, b);
				}
*/
				break;
			}



	}

	return 0;
}




/**
 * print debug info for message elements
 */

/*
void cw_dbg_elem_(struct conn * conn, int msg, int msgelem, const uint8_t * msgbuf, int len)
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
		cw_format_vendor(vendor_details, vendor_id, type, msgbuf);
		sprintf(vendorname, "%s/%s/%s",
			cw_strelemp(conn->actions,msgelem),
			(char *) lw_vendor_id_to_str(vendor_id), vendor_details);
		elemname = vendorname;

	} else {
		elemname = cw_strelemp(conn->actions,msgelem);
	}


	if (!cw_dbg_is_level(DBG_ELEM_DMP))
		cw_dbg(DBG_ELEM,
		       "%s, CAWPAP element: %d (%s), len=%d%s",
		       cw_strmsg(msg), msgelem, elemname, len, "");

	else
		cw_dbg_dmp(DBG_ELEM, msgbuf, len,
			   "%s, CAPWAP element: %d (%s), len=%d%s\n\tDump ...",
			   cw_strmsg(msg), msgelem, elemname, len, "");
}

*/
