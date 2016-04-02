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


#include "cw.h"
#include "log.h"
#include "capwap_items.h"

int cw_in_wtp_reboot_statistics(struct conn *conn, struct cw_action_in *a, uint8_t * data,
			 int len, struct sockaddr *from)
{
	mbag_t rs = mbag_create();
	if (!rs) {
		cw_log(LOG_ERR,"Can't create REBOOT_STATISITCS");
		return 0;
	}		


	mbag_set_word( rs, CW_ITEM_REBOOT_COUNT,cw_get_word(data+0) );
	mbag_set_word( rs, CW_ITEM_REBOOT_AC_INITIATED_COUNT,cw_get_word(data+2) );
	mbag_set_word( rs, CW_ITEM_REBOOT_LINK_FAILURE_COUNT,cw_get_word(data+2) );
	mbag_set_word( rs, CW_ITEM_REBOOT_SW_FAILURE_COUNT,cw_get_word(data+4) );
	mbag_set_word( rs, CW_ITEM_REBOOT_HW_FAILURE_COUNT,cw_get_word(data+6) );
	mbag_set_word( rs, CW_ITEM_REBOOT_OTHER_FAILURE_COUNT,cw_get_word(data+8) );
	mbag_set_word( rs, CW_ITEM_REBOOT_UNKNOWN_FAILURE_COUNT,cw_get_word(data+10) );
	mbag_set_byte( rs, CW_ITEM_REBOOT_LAST_FAILURE_TYPE,cw_get_byte(data+12) );


	mbag_set_mbag(conn->incomming,CW_ITEM_WTP_REBOOT_STATISTICS,rs);

	return 1;

}

