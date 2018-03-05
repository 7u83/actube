#include "format.h"
#include "sock.h"
#include "conn.h"
#include "cw.h"

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

/*		
	if (cw_get_hdr_flag_f(packet)){
		s+=sprintf(s," (fragmented)");
	}
*/
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
		/* print wireless specific info */
		int ws_len = cw_get_hdr_ws_len(packet);
		int plen = ws_len > 20 ? 20 : ws_len;
		s += sprintf(s, " WS:");
		s += format_hexu(s, cw_get_hdr_ws_data(packet), plen);
		if (ws_len > 20) {
			s += sprintf(s, " ... (len=%d)", ws_len);
		}
	}
	return s - dst;



      abort:
	s += sprintf(s, " Incomplete...");
	return s - dst;

}
