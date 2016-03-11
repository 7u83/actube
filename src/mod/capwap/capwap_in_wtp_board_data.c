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



#include "cw/cw.h"
#include "cw/capwap_items.h"

#include "cw/mbag.h"

#include "cw/cw_util.h"
#include "cw/dbg.h"



static void readsubelems_wtp_board_data(mbag_t itemstore, uint8_t * msgelem,
					int len)
{
	if (len<4)
		return;

	int i = 0;
	uint32_t val;
	do {
		val = ntohl(*((uint32_t *) (msgelem + i)));
		int subtype = (val >> 16) & 0xffff;
		int sublen = val & 0xffff;
		i += 4;
		if (sublen + i > len) {
			cw_dbg(DBG_ELEM_ERR,
			       "WTP Board data sub-element too long, type=%d,len=%d",
			       subtype, sublen);
			return;
		}

		cw_dbg(DBG_SUBELEM, "Reading WTP board data sub-element, type=%d, len=%d",
		       subtype, sublen);

		switch (subtype) {
			case CW_BOARDDATA_MODELNO:
				mbag_set_bstrn(itemstore,
						       CW_ITEM_WTP_BOARD_MODELNO,
						       msgelem + i, sublen);
				break;
			case CW_BOARDDATA_SERIALNO:
				mbag_set_bstrn(itemstore,
						       CW_ITEM_WTP_BOARD_SERIALNO,
						       msgelem + i, sublen);

				break;
			case CW_BOARDDATA_MACADDRESS:
				mbag_set_bstrn(itemstore,
						       CW_ITEM_WTP_BOARD_MACADDRESS,
						       msgelem + i, sublen);

				break;
			case CW_BOARDDATA_BOARDID:
				mbag_set_bstrn(itemstore, CW_ITEM_WTP_BOARD_ID,
						       msgelem + i, sublen);
				break;
			case CW_BOARDDATA_REVISION:
				mbag_set_bstrn(itemstore,
						       CW_ITEM_WTP_BOARD_REVISION,
						       msgelem + i, sublen);
			default:
				break;
		}
		i += sublen;

	} while (i < len);
}


/**
 * Parse a WTP Board Data messag element and put results to itemstore.
 */
int capwap_in_wtp_board_data(struct conn *conn, struct cw_action_in *a, uint8_t * data,
			 int len, struct sockaddr *from)
{

	if (len < 4) {
		cw_dbg(DBG_ELEM_ERR,
		       "Discarding WTP_BOARD_DATA msgelem, wrong size, type=%d, len=%d",
		       a->elem_id, len);
		return 0;
	}

	mbag_t itemstore = conn->incomming;
	mbag_set_dword(itemstore, CW_ITEM_WTP_BOARD_VENDOR, cw_get_dword(data));

	readsubelems_wtp_board_data(itemstore, data + 4, len - 4);

	return 1;
}

