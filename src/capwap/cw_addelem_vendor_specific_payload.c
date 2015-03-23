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
 *@file
 *@brief cw_addelem_vendor_specific_payload
 */

#include "capwap.h"

/**
 * Add a Vendor Specific Payload message element to a buffer
 * @param dst destination buffer
 * @param vendor_id the vendor id
 * @param type vendor specific message type
 * @data pointer to data the will be put
 * @len length of data
 */
int cw_addelem_vendor_specific_payload(uint8_t * dst, uint32_t vendor_id,
				       uint16_t type, uint8_t * data, uint16_t len)
{
	uint8_t *d = dst + 4;

	d += cw_put_dword(d, vendor_id);
	d += cw_put_word(d, type);
	d += cw_put_data(d, data, len);

	int l = d - dst;
	cw_put_elem_hdr(dst, CW_ELEM_VENDOR_SPECIFIC_PAYLOAD, l - 4);
	return l;

}
