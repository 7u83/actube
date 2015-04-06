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
 *@brief prototypes for format-functions
 */

#ifndef __CW_FORMAT_H
#define __CW_FORMAT_H


extern int cw_format_hex_bytes(char *dst, const char *format, const char *delim,
			   const uint8_t * src, int len);


/**
 * Format bytes as a hex string. Hexadecimal letters are lower-case.
 * @param dst destination buffer
 * @param bytes bytes to format
 * @param len number of bytes to format
 * @return number of characters written to the destination buffer 
 *
 * Make sure, the destination buffer can hold at least len * 2 characters + the trailing
 * zero for strings.
 */
#define cw_format_hexl(dst,bytes,len)\
	cw_format_hex_bytes(dst,"%02x","",bytes,len)

/** 
 * Format bytes as hex string. Same as #cw_format_hexl, but
 * hexadecimal letters are upper-case.
 */
#define cw_format_hexu\
	cw_format_hex_bytes(dst,"%02X","",bytes,len)

/**
 * Alias for #cw_format_hexl.
 */
#define cw_format_hex cw_format_hexl


#endif
