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

#include "capwap.h"
#include "dbg.h"
#include "sock.h"

#include "log.h"
#include "strlist.h"
#include "format.h"
#include "capwap.h"


void (*cw_dbg_cb) (int level, const char *format, ...) = CW_LOG_DEFAULT_LOG;
void (*cw_dbg_vcb) (int level, const char *fromat, va_list args) = CW_LOG_DEFAULT_VLOG;



uint32_t cw_dbg_opt_display = 0;
uint32_t cw_dbg_opt_level = 0;



/**
 * Put a list of missing mandatory message elements to debug output
 */
void cw_dbg_missing_mand(int level, struct conn *conn, cw_action_in_t ** ml, int n,
			 cw_action_in_t * a)
{
	if (!cw_dbg_is_level(DBG_ELEM) || n == 0)
		return;

	char buffer[2000];
	char *p = buffer;
	int i;
	char *delim = "";
	for (i = 0; i < n; i++) {
		p += sprintf(p, "%s", delim);
		delim = ", ";
		p += sprintf(p, "%s", cw_strelemp(conn->actions, ml[i]->elem_id));
	}
	cw_dbg(DBG_ELEM, "Missing mandatory elements: [%s]", buffer);
}

int cw_format_pkt(char *dst,int level,struct conn *conn, uint8_t * packet, int len)
{
	char *s=dst;
	switch (level) {
		case DBG_PKT_IN:
			s+=sprintf(s,"From %s",sock_addr2str(&conn->addr));
			break;
		case DBG_PKT_OUT:
			s+=sprintf(s,"To %s",sock_addr2str(&conn->addr));
			break;
	}
	s+=sprintf(s," l=%d: ",len);

	int preamble = cw_get_hdr_preamble(packet); 
	if (preamble==01){
		s+=sprintf(s," (encrypted)");
		return s-dst;
	}

	if (len<4)
		goto abort;
		
		
	if (cw_get_hdr_flag_f(packet)){
		s+=sprintf(s," (fragmented)");
	}

	int hlen = cw_get_hdr_hlen(packet); 
	int rid = cw_get_hdr_rid(packet);
	int wbid = cw_get_hdr_wbid(packet);
	s+=sprintf(s," H:%d R:%02d W:%02d",hlen,rid,wbid);


	s+=sprintf(s," Flgs:");
	s+=cw_format_hdr_flags(s,packet);

	if (len<8)
		goto abort;
	int frag_id = cw_get_hdr_fragid(packet);
	int frag_offs = cw_get_hdr_fragoffset(packet);
	s+=sprintf(s," Frag/Offs: %d/%d",frag_id,frag_offs);

	return s-dst;	


	
abort:
	s+=sprintf(s," Incomplete...");
	return s-dst;

}

void cw_dbg_pkt(int level,struct conn *conn, uint8_t * packet, int len)
{
	if (!cw_dbg_is_level(level))
		return;

	char buf[1024];
	cw_format_pkt(buf,level,conn,packet,len);
	cw_dbg(level,"%s",buf);
}


void cw_dbg_msg(int level,struct conn *conn, uint8_t * packet, int len)
{
	if (!cw_dbg_is_level(level))
		return;

	char buf[1024];
	char *s = buf;



	uint8_t * msgptr = cw_get_hdr_msg_ptr(packet);
	int pplen = len - (msgptr-packet);

	int msg_id=cw_get_msg_id(msgptr);
	s+=sprintf(s,"%s Message type %d",cw_strmsg(msg_id),msg_id);
	s+=sprintf(s,"from %s ",sock_addr2str(&conn->addr));
	s+=sprintf(s,", Seqnum: %d ElemLen: %d",cw_get_msg_seqnum(msgptr),cw_get_msg_elems_len(msgptr));

abort:
	cw_dbg(level,"%s",buf);
}




void cw_dbg_packet_m(struct conn *conn, uint8_t * packet, int len)
{
	if (!cw_dbg_is_level(DBG_PKT_IN | DBG_PKT_OUT))
		return;


	/* print the header */
	char hdr[200];
	hdr_print(hdr, packet, len);


	if (!cw_dbg_is_level(DBG_PKT_DMP)) {
		cw_dbg(DBG_PKT_IN, "Processing capwap packet from %s, len=%d\n%s",
		       sock_addr2str(&conn->addr), len, hdr);
		return;

	}

	cw_dbg_dmp(DBG_PKT_DMP, packet, len,
		   "Processing packet from %s, len=%d\n%s\n\tDump:",
		   sock_addr2str(&conn->addr), len, hdr);

}


void cw_dbg_dmp_(int level, const char *file, int line,
		     const uint8_t * data, int len, const char *format, ...)
{
return;
	if (!(level & cw_dbg_opt_level))
		return;


	int maxtlen = 2048;
	int i;
	int rowlen = CW_LOG_DUMP_ROW_LEN;
	int rows = len / rowlen;
	int tlen = 0;

	int md;
	if (cw_dbg_opt_display & DBG_DISP_ASC_DMP)
		md = 2;
	else
		md = 1;



	char *dst = malloc(2*(md * (len * 3 + (rows * 2) + 8 + maxtlen)));
	if (!dst)
		return;

	if (format != NULL) {
		va_list args;
		va_start(args, format);
		tlen = vsnprintf(dst, maxtlen, format, args);
		va_end(args);
	}



	if (len % CW_LOG_DUMP_ROW_LEN)
		rows++;



	char *pdst = dst + tlen;
	sprintf(pdst, "\n\t");
	pdst += 2;

	char asc_buffer[128];
	char *ascdst = asc_buffer;

	for (i = 0; i < len; i++) {
		sprintf(pdst, "%02X ", data[i] & 0xff);

		if (cw_dbg_opt_display & DBG_DISP_ASC_DMP) {
			int c = data[i] & 0xff;
			if (c < 0x20 || c > 0x80)
				c = '.';
			*ascdst = c;
			ascdst++;
		}

		pdst += 3;
		if ((i + 1) % rowlen == 0) {
			int l;
			if (cw_dbg_opt_display & DBG_DISP_ASC_DMP) {
				*ascdst = 0;
				l = sprintf(pdst, " | %s\n\t", asc_buffer);
				ascdst = asc_buffer;

			} else {
				l = sprintf(pdst, "\n\t");
			}
			pdst += l;
		}

	}

	if (cw_dbg_opt_display & DBG_DISP_ASC_DMP) {
		*ascdst = 0;
		if (strlen(asc_buffer))
			pdst += sprintf(pdst, " | %s", asc_buffer);

	}

	if (cw_dbg_opt_display & DBG_DISP_LINE_NUMBERS)
		cw_log_cb(LOG_DEBUG, "%s:%d: %s", file, line, dst);
	else{
		cw_log_cb(LOG_DEBUG, "%s",dst);
	}

	free(dst);
	return;
}




/**
 * print debug info for message elements
 */

void cw_dbg_elem_colored(int level, struct conn *conn, int msg, int msgelem,
			 const uint8_t * msgbuf, int len)
{
	if (!cw_dbg_is_level(level))
		return;

	const char *elemname;
	char vendorname[256];
	char vendor_details[265];
	*vendor_details = 0;

	if (msgelem == CW_ELEM_VENDOR_SPECIFIC_PAYLOAD) {
		uint32_t vendor_id = ntohl(*((uint32_t *) msgbuf));
		int type = ntohs(*((uint16_t *) (msgbuf + 4)));
		cw_format_vendor(vendor_details, vendor_id, type, msgbuf);
		sprintf(vendorname, "%s/%s/%s",
			cw_strelemp(conn->actions, msgelem),
			(char *) lw_vendor_id_to_str(vendor_id), vendor_details);
		elemname = vendorname;

	} else {
		elemname = cw_strelemp(conn->actions, msgelem);
	}


	if (!cw_dbg_is_level(DBG_ELEM_DMP))
		cw_dbg(DBG_ELEM,
		       "%s, CAWPAP element: %d (%s), len=%d%s",
		       cw_strmsg(msg), msgelem, elemname, len, "");

	else
		cw_dbg_dmp(DBG_ELEM, msgbuf, len,
			   "%s, CAPWAP element: %d (%s), len=%d%s\n\tDump ...",
			   cw_strmsg(msg), msgelem, elemname, len, "");
}





static struct cw_str color_on[] = {
	{ DBG_PKT_IN, "\x1b[33m" },
	{ CW_STR_STOP, "" } 
};
static struct cw_str color_ontext[] = {

	{ CW_STR_STOP, "" } 
};


static struct cw_str color_off[] = {

	{ CW_STR_STOP, "\033[22;39m" } 
};

static struct cw_str prefix[] = {
	{ DBG_INFO, " Info -" },
	{ DBG_PKT_IN, " Pkt IN -" },
	{ CW_STR_STOP, "" } 
};




static const char * get_dbg_color_on(int level){
	if ( ! (cw_dbg_opt_display & DBG_DISP_COLORS ) )
		return "";
	return cw_strlist_get_str(color_on,level);
}

static const char * get_dbg_color_off(int level){
	if ( ! (cw_dbg_opt_display & DBG_DISP_COLORS ) )
		return "";
	return cw_strlist_get_str(color_off,level);
}

static const char * get_dbg_prefix(int level){
	return cw_strlist_get_str(prefix,level);

}

static const char * get_dbg_color_ontext(int level){
	if ( ! (cw_dbg_opt_display & DBG_DISP_COLORS ) )
		return "";
	return cw_strlist_get_str(color_ontext,level);

}



/*

void cw_log_colored(int level, const char *format, ...)
{
	char fbuf[1024];

	sprintf(fbuf, "%s%s%s: %s%s",
		get_log_color_on(level),
		get_log_prefix(level),
		get_log_color_ontext(level),
		format,
		get_log_color_off(level)
		);
		

	va_list args;
	va_start(args, format);
	cw_log_vcb(level,fbuf,args);
	va_end(args);

}

*/




void cw_dbg_colored(int level, const char *file, int line, const char *format, ...)
{

	if (!(level & cw_dbg_opt_level))
		return;

	char fbuf[1024];

	sprintf(fbuf, "DBG:%s%s %s%s%s",
		get_dbg_color_on(level),
		get_dbg_prefix(level),
		get_dbg_color_ontext(level),
		format,
		get_dbg_color_off(level)
		);
		

	va_list args;
	va_start(args, format);
	cw_log_vcb(level,fbuf,args);
	va_end(args);


return;



	char buf[2048];

//	va_list args;
	va_start(args, format);
	vsprintf(buf, format, args);
	va_end(args);

	if (cw_dbg_opt_display & DBG_DISP_LINE_NUMBERS)
		cw_log(LOG_DEBUG, "%s:%d: %s", file, line, buf);
	else
		cw_log(LOG_DEBUG, buf);
}


