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
 * @brief
 */

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

/**
 * Creates a bstr_t string.
 * @param data source data to create the string from
 * @param len length of the string 
 * @return the created bstr_t string.
 * 
 * The bstr_t string returned is allocated by malloc. So remember to free 
 * this resource if you don't need it anymore.
 */
uint8_t * bstr_create(uint8_t *data, uint8_t len)
{
	uint8_t * str = malloc(1+len*sizeof(uint8_t));
	if (!str)	
		return 0;
	*str=len;
	memcpy(str+1,data,len);
	return str;
}
