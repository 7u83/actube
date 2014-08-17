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

int cw_readelem_result_code(uint32_t *result_code, int type, uint8_t * msgelem, int len)
{
	if (type != CWMSGELEM_RESULT_CODE)
		return 0;

	if (len!=4){
		cw_dbg(DBG_CW_RFC,"Result code msg elem has wrong size, type=%d,len=%d",type,len);
		return 0;
	}
	*result_code=ntohl(*((uint32_t*)msgelem));
	return 1;
}

