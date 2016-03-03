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
#include "cw_log.h"

int cw_readelem_mtu_discovery_padding(int type, uint8_t * msgelem, int len)
{
	if (type != CWMSGELEM_MTU_DISCOVERY_PADDING)
		return 0;
	if (!cw_dbg_is_level(DBG_CW_RFC))
		return 1;

	int i, n = 0;
	for (i = 0; i < len; i++) {
		if (msgelem[i] != 0xff)
			n++;
	}
	if (n)
		cw_dbg(DBG_CW_RFC,
		       "MTU discovery padding msgelem contains %d non-0xFF byte(s) out of %d, See RFC 5415.",
		       n, len);
	return 1;
}
