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
 *@brief Prototypes of cw_format-functions
 *@defgroup FORMAT FORMAT 
 *@{
 */

#ifndef __FORMAT_H
#define __FORMAT_H

#include <stdint.h>
#include <stddef.h>

#include "sock.h"

int format_hex_bytes(char *dst, const char *format, const char *delim,
			    const uint8_t * src, int len);

char *format_s_hex_bytes(char *dst, const char *format, const char *delim,
				       const uint8_t * src, int len);



#define format_bin2hex(src,len) (format_s_hex_bytes((char[(len) * 2 + 1]) {0}, "%02X", "", src, len))




extern int format_scan_hex_bytes(uint8_t * dst, const char *s, int len);


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
#define format_hexl(dst,bytes,len)\
	format_hex_bytes(dst,"%02x","",bytes,len)

/** 
 * Format bytes as hex string. Same as #format_hexl, but
 * hexadecimal letters are upper-case.
 */
#define format_hexu(dst,bytes,len)\
	format_hex_bytes(dst,"%02X","",bytes,len)

/**
 * Alias for #format_hexl.
 */
#define format_hex format_hexl

/**
 * Format MAC Address.
 */
#define format_mac(dst,src,len)\
	 format_hex_bytes(dst,"%02x",":",src,len)

int format_hdr_flags(char *dst, uint8_t * th);


int format_is_utf8(unsigned char *str, size_t len);

int format_dot11_fc(char *dst, uint16_t fc);





struct cw_FormatDumpSettings {
	int row_len;
	int marker_distance;
	char marker_char;
	int ascii;

	int inv_len;
	char inv_char;
	
	const char * dump_prefix;
	const char * ascii_prefix;
	const char *newline;
};

char *cw_format_dump(const uint8_t * data, int len, 
	struct cw_FormatDumpSettings *settings);

void cw_format_get_dump_defaults(struct cw_FormatDumpSettings * settings);
int cw_format_pkt_hdr(char *dst, int incomming, uint8_t * packet, int len,
		      struct sockaddr *from);

/**@}*/

#endif
