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
#include "dbg.h"


int cw_in_mtu_discovery_padding(struct conn *conn, struct cw_action_in *a,
				 uint8_t * data, int len,struct sockaddr *from)

{
	int i, n = 0;
	for (i = 0; i < len; i++) {
		if (data[i] != 0xff)
			n++;
	}
	if (n){
		cw_dbg(DBG_RFC,
		       "MTU discovery padding msg elem contains %d non-0xFF byte(s) out of %d, See RFC 5415.",
		       n, len);
		if (conn->strict_capwap)
			return 0;
	}
	return 1;
}
