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

#include "format.h"

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


static int cw_format_pkt_hdr0(char *dst, int incomming, uint8_t * packet, int len,
		      struct sockaddr *from, int draft7)
{
	char sock_buf[SOCK_ADDR_BUFSIZE]; 
	int preamble;
	char *s;
	int hlen, rid, wbid;
	int frag_id,frag_offs;

	s = dst;

	if (incomming){
		if (cw_get_hdr_flag_f(packet)) {
			s += sprintf(s, "Fragment from %s",
				     sock_addr2str_p(from,sock_buf));
		} else {
			s += sprintf(s, "From %s", sock_addr2str_p(from,sock_buf));
		}
	}
	else{
		if (cw_get_hdr_flag_f(packet)) {
			s += sprintf(s, "Fragment to %s", sock_addr2str(from,sock_buf));
		} else {
			s += sprintf(s, "To %s", sock_addr2str(from,sock_buf));
		}
	}
	s += sprintf(s, " l=%d: ", len);

	preamble = cw_get_hdr_preamble(packet);
	if (preamble == 01) {
		s += sprintf(s, " (encrypted)");
		return s - dst;
	}

	if (len < 4)
		goto abort;

	hlen = cw_get_hdr_hlen(packet);
	rid = cw_get_hdr_rid(packet);
	wbid = cw_get_hdr_wbid(packet);
	s += sprintf(s, " H:%d R:%02d W:%02d", hlen, rid, wbid);


	s += sprintf(s, " Flgs:");
	s += format_hdr_flags(s, packet);

	if (len < 8)
		goto abort;
	frag_id = cw_get_hdr_fragid(packet);
	frag_offs = cw_get_hdr_fragoffset(packet);
	s += sprintf(s, " Frag/Offs:%d/%d", frag_id, frag_offs);


	if (cw_get_hdr_flag_m(packet)) {
		/* rmac is present, print the rmac */
		int rmac_len = cw_get_hdr_rmac_len(packet);
		int plen = rmac_len;
		if (rmac_len + 8 > len)
			plen = len - 8;
		if (rmac_len > 10)
			plen = 10;

		s += sprintf(s, " R-MAC:");
		s += format_mac(s, cw_get_hdr_rmac_data(packet), plen);
		if (rmac_len > 10) {
			s += sprintf(s, " ... (len=%d)", rmac_len);
		}
	}

	if (cw_get_hdr_flag_w(packet)) {
		if (!draft7){
			/* print wireless specific info */
			int ws_len = cw_get_hdr_ws_len(packet);
			int plen = ws_len > 20 ? 20 : ws_len;
			s += sprintf(s, " WS:");
			s += format_hexu(s, cw_get_hdr_ws_data(packet), plen);
			if (ws_len > 20) {
				s += sprintf(s, " ... (len=%d)", ws_len);
			}
		}
		else {
			int ws_len = cw_get_hdr_ws_len_7(packet);
			int plen = ws_len > 20 ? 20 : ws_len;
			s += sprintf(s, " WS:");
			s += format_hexu(s, cw_get_hdr_ws_data_7(packet), plen);
			if (ws_len > 20) {
				s += sprintf(s, " ... (len=%d)", ws_len);
			}

		}
	}
	return s - dst;

      abort:
	s += sprintf(s, " Incomplete...");
	return s - dst;

}


/**
 * @brief Format a packet header for debugging purposes
 * @param dst Destination buffer
 * @param incomming True if the packet is an incomming packet, otherweise 0
 * @param packet packet data
 * @param len length of packet data
 * @param from Address from where the packet was received
 * @return Number of bytes written
 */
int cw_format_pkt_hdr(char *dst, int incomming, uint8_t * packet, int len,
		      struct sockaddr *from)
{
	return cw_format_pkt_hdr0(dst, incomming, packet, len,
	      from, 0);

}

int cw_format_pkt_hdr_7(char *dst, int incomming, uint8_t * packet, int len,
		      struct sockaddr *from)
{
	return cw_format_pkt_hdr0(dst, incomming, packet, len,
	      from, 1);

}

