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

/**
 * @file
 * @brief Implent lw_readelem_wtp_name
 */


#include "lwapp.h"

#include "cw_util.h"
#include "log.h"

/**
 * Reads the LWAPP message element WTP Name
 * @param dts where to store the WTP name
 * @param type msg element type, must be #LWMSGELEM_WTP_NAME
 * @param msgelem the msg element
 * @param len length of msg element
 * return 0 No WTP name msg elem\n 1=Success 
 */
int lw_readelem_wtp_name(uint8_t ** dst, int type, uint8_t * msgelem, int len)
{
	if (type != LW_ELEM_WTP_NAME)
		return 0;

	
	if (len>254){
		cw_dbg(DBG_MSG_ERR,"Truncating WTP_NAME msgelem to 254, wrong size, type=%d,len=%d",type,len);
		len=254;
	}

	cw_setstr(dst,msgelem,len);
	return 1;
}


