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

#include "capwap.h"
#include "wtpinfo.h"
#include "conn.h"
#include "log.h"
#include "sock.h"

void conn_detect_capwap(struct conn * conn, struct wtpinfo * wtpinfo)
{
	if (wtpinfo->software_vendor_id == CW_VENDOR_ID_CISCO){
		cw_dbg(DBG_CW_INFO,"Setting capwap mode *Cisco* on connection %s",sock_addr2str(&conn->addr));
		conn->capwap_mode = CWMODE_CISCO;
		return;
	}

	conn->capwap_mode = CWMODE_STD;
		
}
