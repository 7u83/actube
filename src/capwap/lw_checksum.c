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

#include "lwapp.h"

/**
 * Calculate the 16-bit checksum for LWAPP image data message 
 * elements with opcode 3 - used by Cisco also in CAPWAP
 */
uint16_t lw_checksum(uint8_t * d, int len)
{
	int i;
	uint32_t cs = 0xffff;
	for (i = 0; i < len; i += 2) {
		uint16_t w = d[i] << 8;

		if (i + 1 < len)
			w |= d[i + 1];

		cs += w ^= 0xffff;
		cs += cs >> 16;
		cs &= 0xffff;
	}
	return (uint16_t) cs;
}
