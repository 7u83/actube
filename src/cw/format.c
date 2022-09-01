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

#include "cw.h"

#include "capwap80211.h"

/**
 * Format bytes as hex string.
 * @param dst Destination buffer
 * @param format Format to use, when writing a byte. Use the same format syntax as for printf,
 * e.g. the string "%02X" would format a byte as upper case hexadeciaml string.
 * @param delim Delimiter string. The given string is placed between bytes.
 * @param src Bytes to format
 * @param len Number of bytes to format
 * @return the number of characters written to dst
 *
 * This function is used by macros like #cw_format_hexl, #cw_format_hex ... \n
 * The size of the destination buffer must be at least x * len + strlen(delim) * (len-1) +1,
 * where x is the number of characters. A formatted hex byte needs (typically 2).
 *
 */
int format_hex_bytes(char *dst, const char *format, const char *delim,
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




char *format_s_hex_bytes(char *dst, const char *format, const char *delim,
				       const uint8_t * src, int len)
{
	format_hex_bytes(dst, format, delim, src, len);
	return dst;
}



int format_hdr_flags(char *dst, uint8_t * th)
{
	char *s = dst;
	s += sprintf(s, "%s", "(");
	s += sprintf(s, "%s", cw_get_hdr_flag_t(th) ? "T" : "");
	s += sprintf(s, "%s", cw_get_hdr_flag_f(th) ? "F" : "");
	s += sprintf(s, "%s", cw_get_hdr_flag_l(th) ? "L" : "");
	s += sprintf(s, "%s", cw_get_hdr_flag_w(th) ? "W" : "");
	s += sprintf(s, "%s", cw_get_hdr_flag_m(th) ? "M" : "");
	s += sprintf(s, "%s", cw_get_hdr_flag_k(th) ? "K" : "");
	s += sprintf(s, "%s", ")");
	return s - dst;

}

char * cw_format_radio_information(char * dst, int ri)
{
	char *d = dst;	
	if (ri & CW_80211_RADIO_TYPE_A)
		*d++='a';
	if (ri & CW_80211_RADIO_TYPE_B)
		*d++='b';
	if (ri & CW_80211_RADIO_TYPE_G)
		*d++='g';
	if (ri & CW_80211_RADIO_TYPE_N)
		*d++='n';

	*d=0;
	return dst;
}
