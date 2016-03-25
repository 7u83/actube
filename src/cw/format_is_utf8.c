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
 * @brief Implementation for format_is_utf8
 */

#include "format.h"

/**
 * Check if a string is in UTF8 format
 * @param str String to check
 * @param len Length of the string
 * @return 0 if the string is not in UTF8 format, otherwise it is UTF8.
 */

int format_is_utf8(unsigned char *str, size_t len)
{
	size_t i = 0;
	size_t bytes = 0;

	while (i < len) {
		if (str[i] < 0x20) {
			return 0;
		}
		if (str[i] <= 0x7F)
			bytes = 0;
		else if (str[i] >= 0xC0 /*11000000 */  && str[i] <= 0xDF /*11011111 */ )
			bytes = 1;
		else if (str[i] >= 0xE0 /*11100000 */  && str[i] <= 0xEF /*11101111 */ )
			bytes = 2;
		else if (str[i] >= 0xF0 /*11110000 */ 
			 && str[i] <= 0xF4 /* Cause of RFC 3629 */ )
			bytes = 3;
		else {
			return 0;
		}
		i += 1;
		while (i < len && bytes > 0 && str[i] >= 0x80 && str[i] <= 0xBF) {
			i += 1;
			bytes -= 1;
		}
		if (bytes != 0) {
			return 0;
		}
	}
	return 1;
}
