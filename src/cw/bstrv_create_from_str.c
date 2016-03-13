/*
    This file is part of actube.

    actube is free software: you can redistribute it and/or modify
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
 */

#include "bstr.h"
#include "format.h"

/**
 * Create a bstrv variable from string.
 * @param vendor_id Vendor ID
 * @param s string
 * @return the created bstrv variable or NULL if the 
 * operation was not sucessfull
 */
uint8_t * bstrv_create_from_str(uint32_t vendor_id,const char *s)
{
	int l = strlen(s);
	if (s[0]!='.')
		return bstrv_create(vendor_id,(uint8_t*)s,l);

	if (l<=2)
		return bstrv_create(vendor_id,(uint8_t*)s,l);

	if (s[1]=='.')
		return bstrv_create(vendor_id,(uint8_t*)s+1,l-1);

	if (s[1]!='x')
		return bstrv_create(vendor_id,(uint8_t*)s,l);

	/* the string starts with ".x" - read hexbytes */
	l-=2;
	int msize=l/2;	
	if(l&1)
		msize++;


	uint8_t * mem = malloc(bstrv_size(msize));
	if(!mem)
		return NULL;
	bstrv_set_vendor_id(mem,vendor_id);
	bstrv_set_len(mem,msize);

	format_scan_hex_bytes(bstrv_data(mem),s+2,l);
	return mem;		
}


