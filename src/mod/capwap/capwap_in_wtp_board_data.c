/*
    This file is part of libcapwap.

    libcapwap is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libcapwap is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <stdlib.h>
#include <string.h>

#include "cw/conn.h"
#include "mod_capwap.h"

#include "cw/dbg.h"
#include "cw/cw.h"

#include "cw/val.h"

#include "cw/keys.h"


static void readsubelems_wtp_board_data(mavl_t cfg, uint8_t * msgelem,
					int len)
{
	int i = 0;
	uint32_t val;
	const char * key;
	
	if (len<4)
		return;


	do {
		int subtype;
		int sublen;
		
		val = ntohl(*((uint32_t *) (msgelem + i)));
		subtype = (val >> 16) & 0xffff;
		sublen = val & 0xffff;
		i += 4;
		if (sublen + i > len) {
			cw_dbg(DBG_ELEM_ERR,
			       "WTP Board data sub-element too long, type=%d,len=%d",
			       subtype, sublen);
			return;
		}

/*		cw_dbg(DBG_SUBELEM, "WTP board data sub-element, type=%d, len=%d",
		       subtype, sublen);*/

		switch (subtype) {

			
			case CW_BOARDDATA_MODELNO:
				key = "model-no";
				break;
			case CW_BOARDDATA_SERIALNO:
				key = "serial-no";
				break;
			case CW_BOARDDATA_MACADDRESS:
				key = "mac-address";
				break;
			case CW_BOARDDATA_BOARDID:
				key = "board-id";
				break;
			case CW_BOARDDATA_REVISION:
				key = "revision";
				break;
			default:
				key = NULL;
				break;
		}
		if (key != NULL){
			char add_key[CW_CFG_MAX_KEY_LEN];
			sprintf(add_key,"wtp-board-data/%s",key);
			cw_cfg_set_val(cfg,add_key,CW_TYPE_BSTR16,NULL,msgelem+i,sublen);
			
		}
		
		
		i += sublen;

	} while (i < len);
}



/**
 * Parse a WTP Board Data messag element and put results to itemstore.
 */
int capwap_in_wtp_board_data(struct cw_ElemHandler *eh, struct cw_ElemHandlerParams *params, 
			uint8_t * data,	 int len)
{
	char vendor_key[CW_CFG_MAX_KEY_LEN];

	sprintf(vendor_key,"%s/%s",eh->key,"vendor");
	cw_cfg_set_val(params->cfg,vendor_key,CW_TYPE_DWORD,NULL,data,len);

	readsubelems_wtp_board_data(params->cfg, data + 4, len - 4);
	return 1;
}

