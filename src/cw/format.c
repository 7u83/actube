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



/**
 * Read hex bytes from a string to an uint8_t array
 * @param dst destination array
 * @param s string to read
 * @param len length of string
 * @return 0 if all was ok \n 1 if an error has occured.
 */

int format_scan_hex_bytes(uint8_t *dst,const char *s, int len)
{
	int rc ;
	int err=0;
	int val;
	int c;
	int i;

	if ( len & 1){
		rc = sscanf(s,"%01X",&c);
		if (rc!=1){
			c=0;
			err=1;
		}
		*dst++=c;
		s++;
		len--;
	}


	for (i=0; i<len; i++){
		rc = sscanf(s+i,"%01X",&c);
		if (rc!=1){
			c=0;
			err=1;
		}


		if (!(i&1)) {
			val = c<<4;	

		}
		else{
			val |= c;
			*dst++=val;
		}

	}
	
	return err;
}


