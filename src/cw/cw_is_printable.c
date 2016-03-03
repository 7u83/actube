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

#include <stdint.h>

/*
 * Check if a string is printable (human readable)
 */ 
int cw_is_printable(const uint8_t * s,int len)
{
	int i;
	for (i=0; i<len; i++){
		if (s[i]<32 || s[i]>126)
			return 0;
	}
	return 1;
}

