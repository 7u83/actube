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
 * @brief Implementation of lw_put_cisco_path_mtu
 */

#include "lwapp_cisco.h"
#include "lw.h"
#include "vendors.h"

/** 
 * Put message element data for Cisco vendor specific LWAPP message
 * Csico Path MTU.
 * @param dst destination buffer
 * @param max maximum MTU
 * @param padding number of bytes to append
 * @return number of bytes put to buffer 
 */
int lw_put_cisco_path_mtu(uint8_t *dst, uint16_t max, uint16_t padding)
{
	lw_put_dword(dst,LW_VENDOR_ID_CISCO);
	lw_put_word(dst+4,LW_CISCO_PATH_MTU);
	lw_put_word(dst+6,max);
	lw_put_word(dst+8,padding+4);
	memset(dst+10,0,padding);
	return padding+10;
}
