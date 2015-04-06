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
 * @brief Implementation of various format functions.
 */

#include <stdint.h>
#include <stdio.h>

/**
 * Format bytes as hex string.
 * @param dst destination buffer
 * @param format format to use when writing a byte.
 * @param delim delimiter
 * @param src bytes to format
 * @param len number of bytes to format
 * @return the number character written to dst
 *
 * This function is used by macros like #cw_format_hexl, #cw_format_hex ... \n
 * The size of the destination buffer must be at least x * len + strlen(delim) * (len-1) +1,
 * where x is the number of characters a formatted hex byte needs (typically 2).
 *
 */
int cw_format_hex_bytes(char *dst, const char *format, const char *delim,
			const uint8_t * src, int len)
{
	char *d = dst;
	int i;
	const char *cdelim = NULL;
	for (i = 0; i < len; i++) {
		if (cdelim)
			d += sprintf(d, "%s", cdelim);
		else
			cdelim = delim;

		d += sprintf(d, format, src[i]);

	}
	return d - dst;
}
