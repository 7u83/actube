#include <stdint.h>

#include "dbg.h"
#include "format.h"





/**
 * print debug info for message elements
 */

void cw_dbg_elem(int level, struct conn *conn, int msg, 
			struct cw_ElemHandler * handler,
			 const uint8_t * msgbuf, int len)
{


/*	const char *elemname;*/
	char vendorname[256];
	char vendor_details[265];

	
	if (!cw_dbg_is_level(level))
		return;
		
	*vendor_details = 0;

/*	handler = cw_msgset_get_elemhandler(conn->msgset,0,0,msgelem);
	if (!handler)
		return;
*/

	
/*	
/// TODO XXXX
*/
/*	if (msgelem == -1) {
		uint32_t vendor_id = ntohl(*((uint32_t *) msgbuf));
		int type = ntohs(*((uint16_t *) (msgbuf + 4)));
		cw_format_vendor(vendor_details, vendor_id, type, msgbuf);
		elemname = vendorname;

	} else {
		elemname=handler->name;
	}
*/
	if (handler->vendor){
		sprintf(vendorname,"Vendor %s",cw_strvendor(handler->vendor));
	}
	else{
		sprintf(vendorname,"");
	}


	if (cw_dbg_is_level(DBG_ELEM_DMP)) {
		/*
		char *dmp = cw_dbg_mkdmp(msgbuf, len);
		 */
		char *dmp;
		dmp = cw_format_dump(msgbuf,len,NULL);

		cw_dbg(DBG_ELEM, "%s %d (%s), len=%d%s%s",
			vendorname,
		       handler->id, handler->name, len, /*get_dbg_color_ontext(DBG_ELEM_DMP)*/ "", dmp);

		free(dmp);
	}
	else
		cw_dbg(DBG_ELEM, "%s %d (%s), len=%d", 
		vendorname,
		handler->id, handler->name, len);

}


