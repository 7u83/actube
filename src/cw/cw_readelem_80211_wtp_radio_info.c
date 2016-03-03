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

#include <arpa/inet.h>

#include "wtpinfo.h"
#include "capwap_80211.h"

#include "cw_log.h"


int cw_readelem_80211_wtp_radio_info(void *dst,int type,uint8_t *msgelem, int len)
{
	if (type != CWMSGELEM_80211_WTP_RADIO_INFO)
		return 0;

	if (len!=5){
		cw_dbg(DBG_MSG_ERR,"Discarding msgelem 80211_WTP_RADIO_INFO, wrong size, len=%d\n",len);
		return 0;
	}

	if (*msgelem <1 || *msgelem>31){
		cw_dbg(DBG_CW_RFC,"Non-standard conform radio id, val=%d (must be between 1 to 31, See RFC 5415");
	}


//	int rid=*msgelem & 0x1f;

/*	
	wtpinfo->radioinfo[rid].set=1;
	wtpinfo->radioinfo[rid].rid=rid;
	wtpinfo->radioinfo[rid].type=ntohl(*(uint32_t*)(msgelem+1));
*/
	return 1;
}

