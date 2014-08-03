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

int cw_readelem_ecn_support(uint8_t *ecn_support, int type, uint8_t * msgelem, int len)
{
	if (type != CWMSGELEM_ECN_SUPPORT)
		return 0;

	if (len!=1){
		cw_log_debug0("Discarding ECN_SUPPORT msgelem, wrong size, type=%d,len=%d",type,len);
		return 1;
	}
	*ecn_support=*msgelem;
	return 1;
}


