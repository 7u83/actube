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
#include "cw_util.h"
#include "cw_log.h"

int cw_readelem_ac_name(uint8_t **dst, int type,uint8_t *msgelem, int len)
{
	if (type != CW_ELEM_AC_NAME)
		return 0;

	if (len > 512 ) {
		cw_dbg(DBG_CW_RFC,"AC name too long, max. 512 bytes allowed (RFC 5415)");
		cw_dbg(DBG_MSG_ERR,"AC name truncated to 512 bytes");
		len = 512;
	}

	cw_setstr(dst,msgelem,len);
	return 1;
}


