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
 * @file Debugging Functions
 * @brief Various debug functions.
 */

#include "capwap.h"
#include "dbg.h"
#include "sock.h"

#include "log.h"
#include "strlist.h"
#include "format.h"
#include "capwap.h"
#include "capwap_cisco.h"
#include "lwapp_cisco.h"

/**
 *@addtogroup DBG
 *@{
 */

 /*
 * @defgroup DebugFunctions Debug Functions
 * @{
 */ 



void (*cw_dbg_cb) (int level, const char *format, ...) = CW_LOG_DEFAULT_LOG;
void (*cw_dbg_vcb) (int level, const char *fromat, va_list args) = CW_LOG_DEFAULT_VLOG;



uint32_t cw_dbg_opt_display = 0;

/**
 * Current debug level
 */ 
uint32_t cw_dbg_opt_level = 0;


#define DBG_CLR_MAGENTA "\x1b[35m"
#define DBG_CLR_MAGENTA_L "\x1b[95m"

#define DBG_CLR_MAGENTA_B "\x1b[1;35m"
#define DBG_CLR_MAGENTA_F "\x1b[2;35m"
#define DBG_CLR_MAGENTA_I "\x1b[3;35m"

#define DBG_CLR_BLUE	"\x1b[34m"
#define DBG_CLR_BLUE_B	"\x1b[1;34m"
#define DBG_CLR_BLUE_F	"\x1b[2;34m"
#define DBG_CLR_BLUE_I	"\x1b[3;34m"

#define DBG_CLR_YELLO	"\x1b[33m"
#define DBG_CLR_YELLO_I "\x1b[3;33m"

#define DBG_CLR_CYAN	"\x1b[36m"


#define DBG_CLR_RED_I	"\x1b[3;31m"


static struct cw_strlist_elem color_on[] = {
	{ DBG_PKT_IN, DBG_CLR_YELLO },
	{ DBG_PKT_OUT, DBG_CLR_YELLO_I },

	{ DBG_MSG_IN, DBG_CLR_BLUE },
	{ DBG_MSG_OUT, DBG_CLR_BLUE_I },

	{ DBG_ELEM, "\x1b[39m" },
	{ DBG_MSG_ERR, "\x1b[31m" },
	{ DBG_PKT_ERR, "\x1b[31m" },
	{ DBG_ELEM_ERR, "\x1b[31m" },
	{ DBG_SUBELEM, "\x1b[30m"},
	{ DBG_DTLS, DBG_CLR_MAGENTA_B },
	{ DBG_DTLS_DETAIL, DBG_CLR_MAGENTA },
	{ DBG_DTLS_BIO,DBG_CLR_MAGENTA_L},


	{ DBG_RFC, "\x1b[31m" },
	{ DBG_X, "\x1b[31m" },
	{ DBG_WARN, DBG_CLR_CYAN },
	{ DBG_MOD, "\x1b[91m" },
	{ CW_STR_STOP, "" } 
};
static struct cw_strlist_elem color_ontext[] = {

	{ DBG_ELEM_DMP, "\x1b[30m"},
	{ CW_STR_STOP, "" } 
};


static struct cw_strlist_elem color_off[] = {

	{ CW_STR_STOP, "\x1b[22;39m\x1b[23m" } 
};

static struct cw_strlist_elem prefix[] = {
	{ DBG_INFO, " Info -" },
	{ DBG_PKT_IN, " Pkt IN -" },
	{ DBG_PKT_OUT, " Pkt Out -" },
	{ DBG_MSG_IN, " Msg IN -" },
	{ DBG_MSG_OUT, " Msg Out -" },

	{ DBG_ELEM,   " Msg Element -" },
	{ DBG_MSG_ERR," Msg Error -" },
	{ DBG_PKT_ERR," Pkt Error -" },
	{ DBG_ELEM_ERR," Elem Error -" },
	{ DBG_RFC,    " RFC -" },
	{ DBG_SUBELEM," Sub-Element - "},
	{ DBG_DTLS, " DTLS - "},
	{ DBG_DTLS_DETAIL, " DTLS - "},
	{ DBG_WARN, " Warning - "},
	{ DBG_MOD, " Mod - "},
	{ DBG_X, "XXXXX - "},

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









/**
 * Put a list of missing mandatory message elements to debug output
 */
void cw_dbg_missing_mand(int level, struct conn *conn, cw_action_in_t ** ml, int n,
			 cw_action_in_t * a)
{
//	if (!cw_dbg_is_level(DBG_MSG_ERR) || n == 0)
//		return;

	if ( !cw_dbg_is_level(level) || n==0)
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
	cw_dbg(level, "Missing mandatory elements: [%s]", buffer);
}

/**
 * Format a Packet Header
 */
int cw_format_pkt_hdr(char *dst,int level,struct conn *conn, uint8_t * packet, int len,struct sockaddr *from)
{
	char *s=dst;
	switch (level) {
		case DBG_PKT_IN:
			if (cw_get_hdr_flag_f(packet)){
				s+=sprintf(s,"Fragment from %s",sock_addr2str_p(from));
			}
			else{
				s+=sprintf(s,"From %s",sock_addr2str_p(from));
			}
			break;
		case DBG_PKT_OUT:
			if (cw_get_hdr_flag_f(packet)){
				s+=sprintf(s,"Fragment to %s",sock_addr2str(from));
			}
			else{
				s+=sprintf(s,"To %s",sock_addr2str(from));
			}
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
		
/*		
	if (cw_get_hdr_flag_f(packet)){
		s+=sprintf(s," (fragmented)");
	}
*/
	int hlen = cw_get_hdr_hlen(packet); 
	int rid = cw_get_hdr_rid(packet);
	int wbid = cw_get_hdr_wbid(packet);
	s+=sprintf(s," H:%d R:%02d W:%02d",hlen,rid,wbid);


	s+=sprintf(s," Flgs:");
	s+=format_hdr_flags(s,packet);

	if (len<8)
		goto abort;
	int frag_id = cw_get_hdr_fragid(packet);
	int frag_offs = cw_get_hdr_fragoffset(packet);
	s+=sprintf(s," Frag/Offs:%d/%d",frag_id,frag_offs);


	if (cw_get_hdr_flag_m(packet)) {
		/* rmac is present, print the rmac */
		int rmac_len=cw_get_hdr_rmac_len(packet);
		int plen=rmac_len;
		if (rmac_len+8>len) 
			plen=len-8;
		if (rmac_len>10) 
			plen=10;
		
		s+=sprintf(s," R-MAC:");
		s+=format_mac(s,cw_get_hdr_rmac_data(packet),plen);
		if (rmac_len>10){
			s+=sprintf(s," ... (len=%d)",rmac_len);
		}
	}

	if (cw_get_hdr_flag_w(packet)){
		/* print wireless specific info */
		int ws_len = cw_get_hdr_ws_len(packet);
		int plen = ws_len > 20 ? 20:ws_len;
		s+=sprintf(s," WS:");
		s+=format_hexu(s,cw_get_hdr_ws_data(packet),plen);
		if (ws_len>20){
			s+=sprintf(s," ... (len=%d)",ws_len);
		}
	}

	

	return s-dst;	


	
abort:
	s+=sprintf(s," Incomplete...");
	return s-dst;

}

/**
 * Create an ASCII hex dump of binary data
 * 
 * @param data data to dump
 * @param len number of bytes to dump (size of data)
 * @return a character string with the created data ASCII dump (must be released with free)
 */ 
char * cw_dbg_mkdmp_c( const uint8_t * data, int len, int invlen)
{

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
		return NULL;

	if (len % CW_LOG_DUMP_ROW_LEN)
		rows++;


	char *pdst = dst + tlen;
	sprintf(pdst, "\n\t");
	pdst += 2;

	char asc_buffer[128];
	char *ascdst = asc_buffer;

//	if (invlen) {
//		pdst+=sprintf(pdst,"\x1b[7m");
//	}

	for (i = 0; i < len; i++) {
		char * sp=" ";
		if(i==invlen-1)
			sp="|";
//		if (i==invlen){
//			pdst+=sprintf(pdst,"\x1b[27m");
//		}

		pdst+=sprintf(pdst, "%02X%s", data[i] & 0xff,sp);
		if (cw_dbg_opt_display & DBG_DISP_ASC_DMP) {
			int c = data[i] & 0xff;
			if (c < 0x20 || c > 0x7f)
				c = '.';
			*ascdst = c;
			ascdst++;
		}

//		pdst += 3;
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

	return dst;
}




char * cw_dbg_mkdmp( const uint8_t * data, int len)
{
	return cw_dbg_mkdmp_c(data,len,0);
}














/**
 * Display a packet on for debugger
 */ 
void cw_dbg_pkt(int level,struct conn *conn, uint8_t * packet, int len,struct sockaddr *from)
{
	if (!cw_dbg_is_level(level))
		return;

	char buf[1024];
	cw_format_pkt_hdr(buf,level,conn,packet,len,from);

	int hlen = cw_get_hdr_msg_offset(packet);

	if (cw_dbg_is_level(DBG_PKT_DMP)){
		char  *dmp = cw_dbg_mkdmp_c(packet,len,hlen);
		cw_dbg(level,"%s%s",buf,dmp);
		free(dmp);
	}
	else
		cw_dbg(level,"%s",buf);
}

void cw_dbg_pkt_nc(int level,struct netconn *nc, uint8_t * packet, int len,struct sockaddr *from)
{
	if (!cw_dbg_is_level(level))
		return;

	char buf[1024];
	cw_format_pkt_hdr(buf,level,NULL,packet,len,from);

	int hlen = cw_get_hdr_msg_offset(packet);

	if (cw_dbg_is_level(DBG_PKT_DMP)){
		char  *dmp = cw_dbg_mkdmp_c(packet,len,hlen);
		cw_dbg(level,"%s%s",buf,dmp);
		free(dmp);
	}
	else
		cw_dbg(level,"%s",buf);
}






void cw_dbg_dmp_(int level, const char *file, int line,
		     const uint8_t * data, int len, const char *format, ...)
{
	if (!cw_dbg_is_level(level))
		return;


	char  *dmp = cw_dbg_mkdmp(data,len);
	cw_dbg(level,"%s%s",format,dmp);
	free(dmp);


}




void cw_dbg_msg(int level,struct conn *conn, uint8_t * packet, int len,struct sockaddr *from)
{
	if (!cw_dbg_is_level(level))
		return;

	char buf[1024];
	char *s = buf;



	uint8_t * msgptr = cw_get_hdr_msg_ptr(packet);
//	int pplen = len - (msgptr-packet);

	int msg_id=cw_get_msg_id(msgptr);
	s+=sprintf(s,"%s Message (type=%d) ",cw_strmsg(msg_id),msg_id);
	if ( level == DBG_MSG_IN ) 
		s+=sprintf(s,"from %s ",sock_addr2str(from));
	else
		s+=sprintf(s,"to %s ",sock_addr2str(from));

	s+=sprintf(s,", Seqnum: %d ElemLen: %d",cw_get_msg_seqnum(msgptr),cw_get_msg_elems_len(msgptr));

//abort:
	cw_dbg(level,"%s",buf);
}

static int cw_format_vendor(char *dst, uint32_t vendor_id, int elem_id,
		     const uint8_t * elem_data)
{
	switch (vendor_id) {
		case CW_VENDOR_ID_CISCO:
			{
				if (elem_id != CW_CISCO_SPAM_VENDOR_SPECIFIC) {
					return sprintf(dst, "%d - %s", elem_id,
						       cw_cisco_id_to_str(elem_id));
				}


				/* dive into LWAPP vendor specific decoding */
				uint32_t lw_elem_id = lw_get_word(elem_data + 4 + 6);
				return sprintf(dst, "%d/LWAPP Vendor: %d - %s",
					       elem_id,
					       lw_elem_id,
					       lw_cisco_id_to_str(lw_elem_id));



				break;
			}
		default:
			{
					return sprintf(dst, "%d", elem_id);
		
			}



	}

	return 0;
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
			(char *) cw_strvendor(vendor_id), vendor_details);
		elemname = vendorname;

	} else {
		elemname = cw_strelemp(conn->actions, msgelem);
	}


	if (!cw_dbg_is_level(DBG_ELEM_DMP)){
		cw_dbg(DBG_ELEM, "%d (%s), len=%d",
		       msgelem, elemname, len);
	}
	else{
		char *dmp = cw_dbg_mkdmp(msgbuf,len);

		cw_dbg(DBG_ELEM, "%d (%s), len=%d%s%s",
			msgelem, 
			elemname, 
			len, 
			get_dbg_color_ontext(DBG_ELEM_DMP),
			dmp);

		free(dmp);
	}
}


void cw_dbg_colored(int level, const char *file, int line, const char *format, ...)
{

	if (!(cw_dbg_is_level(level)))
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

}

//int cw_is_utf8(unsigned char *str, size_t len);


int cw_format_item(char *dst,mbag_item_t * item)
{
	*dst=0;
	if (item->type==MBAG_BSTR16){
		strncpy(dst,(char*)bstr16_data(item->data),bstr16_len(item->data));
		*(dst+bstr16_len(item->data))=0;
		return bstr16_len(item->data);


	}
	return 0;
}

static int cw_format_version(char *s, bstrv_t ver, char * def)
{
	if (!ver)
		return sprintf(s,"%s",def);


	uint8_t * version = bstrv_data(ver);
	int len = bstrv_len(ver);
	


	int rs=0;	
	int i;


	if ( format_is_utf8(version,len)  ){
		if (len != 0 )
			rs+=sprintf(s+rs,"%.*s",len,version);
		else
			rs+=sprintf(s+rs,"''");
	}
	else{
		for (i=0; i<len && i<20; i++){
			rs+=sprintf(s+rs,"%02X",version[i]);
		}

		int dot=0;

		rs+=sprintf(s+rs," (");
		for (i=0; i<len && i<20; i++){
			if (dot) 
				rs+=sprintf(s+rs,".");
			dot=1;
			rs+=sprintf(s+rs,"%d",version[i]);
		}
		rs+=sprintf(s+rs,")");
	}

	uint32_t vendor = bstrv_get_vendor_id(ver);
	rs+=sprintf(s+rs,", Vendor Id: %d, %s",vendor, cw_strvendor(vendor));
	return rs;	
}



void cw_dbg_version_subelem(int level,const char*context,int subtype,bstrv_t vstr)
{
	if ( !cw_dbg_is_level(level))
		return;
	if (!vstr) 
		return;
	char v[256];
	cw_format_version(v,vstr,"");
	cw_dbg(level,"%s: SubType %d, %s",context,subtype,v);
		
}


/*
void dbg_istore_dmp(mbag_t s)
{
	DEFINE_AVLITER(it,s);
	avliter_foreach(&it) {

		mbag_item_t *i = avliter_get(&it);

		char buffer[1000];

		struct cw_str * strings = cw_item_strings;
		
		const char * in = cw_strlist_get_str(strings,i->id);
		
		cw_format_item(buffer,i);
		printf("Item ID %d-%s: %s\n",i->id,in,buffer);
				

	}


}
*/


/**@}*/


/**@}*/

