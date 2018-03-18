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
#include <stdarg.h>
#include <stdint.h>

#include "dbg.h"
#include "log.h"
#include "format.h"



/**
 *@addtogroup DBG
 *@{
 */

 /*
  * @defgroup DebugFunctions Debug Functions
  * @{
  */

uint32_t cw_dbg_opt_display = DBG_DISP_COLORS;

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
#define DBG_CLR_BLUE_L	"\x1b[94m"
#define DBG_CLR_BLUE_F	"\x1b[2;34m"
#define DBG_CLR_BLUE_I	"\x1b[3;34m"

#define DBG_CLR_YELLO	"\x1b[33m"
#define DBG_CLR_YELLO_I "\x1b[3;33m"

#define DBG_CLR_CYAN	"\x1b[36m"

#define DBG_CLR_RED	"\x1b[31m"
#define DBG_CLR_RED_I	"\x1b[3;31m"
#define DBG_CLR_RED_L	"\x1b[91m"

#define DBG_CLR_DEFAULT "\x1b[39m"
#define DBG_CLR_DEFAULT_I "\x1b[3;39m"

#define DBG_CLR_BLACK	"\x1b[30m"
#define DBG_CLR_BLACK_L	"\x1b[90m"
#define DBG_CLR_BLACK_LI	"\x1b[3;90m"

#define DBG_CLR_WHITE	"\x1b[37m"
#define DBG_CLR_WHITE_I	"\x1b[3;37m"


#define DBG_CLR_OFF	"\x1b[22;39m\x1b[23m"


#define ANSI_DEFAULT	"\x1b[39m"
#define ANSI_BLUE	"\x1b[34m"

#define ANSI_ITALIC	"\x1b[3m"
#define ANSI_BOLD	"\x1b[1m"


static struct cw_StrListElem color_on[] = {
	{DBG_PKT_IN, DBG_CLR_YELLO},
	{DBG_PKT_OUT, DBG_CLR_YELLO_I},

	{DBG_MSG_IN, ANSI_BLUE },
	{DBG_MSG_OUT, ANSI_BLUE ANSI_ITALIC},

	{DBG_ELEM_IN, ANSI_DEFAULT},
	{DBG_ELEM_OUT, ANSI_DEFAULT ANSI_ITALIC},
	
	{DBG_MSG_ERR, DBG_CLR_RED},
	{DBG_PKT_ERR, DBG_CLR_RED},
	{DBG_ELEM_ERR, DBG_CLR_RED},
	
	{DBG_SUBELEM, DBG_CLR_BLACK},
	{DBG_DTLS, DBG_CLR_MAGENTA_B},
	{DBG_DTLS_DETAIL, DBG_CLR_MAGENTA},
	{DBG_DTLS_BIO, DBG_CLR_MAGENTA_L},

	{DBG_INFO, DBG_CLR_DEFAULT},

	{DBG_RFC, DBG_CLR_RED},
	{DBG_X, "\x1b[31m"},
	{DBG_WARN, DBG_CLR_CYAN},
	{DBG_MOD, DBG_CLR_WHITE},
	{CW_STR_STOP, ""}
};

struct cw_StrListElem color_ontext[] = {

/*	{DBG_ELEM_DMP, "\x1b[37m"},*/
	{DBG_ELEM_DMP, DBG_CLR_BLACK_LI},
	{CW_STR_STOP, ""}
};


static struct cw_StrListElem color_off[] = {

	{CW_STR_STOP, "\x1b[22;39m\x1b[23m"}
};

static struct cw_StrListElem prefix[] = {
	{DBG_INFO, " Info -"},
	{DBG_PKT_IN, " Pkt IN -"},
	{DBG_PKT_OUT, " Pkt Out -"},
	{DBG_MSG_IN, " Msg IN -"},
	{DBG_MSG_OUT, " Msg Out -"},

	{DBG_ELEM_IN, " Msg Element -"},
	{DBG_ELEM_OUT, " Msg Element -"},
	
	{DBG_MSG_ERR, " Msg Error -"},
	{DBG_PKT_ERR, " Pkt Error -"},
	{DBG_ELEM_ERR, " Elem Error -"},
	{DBG_RFC, " RFC -"},
	{DBG_SUBELEM, " Sub-Element - "},
	{DBG_DTLS, " DTLS - "},
	{DBG_DTLS_DETAIL, " DTLS - "},
	{DBG_WARN, " Warning - "},
	{DBG_MOD, " Mod - "},
	{DBG_X, "XXXXX - "},

	{CW_STR_STOP, ""}
};

static const char *get_dbg_color_on(int level)
{
	if (!(cw_dbg_opt_display & DBG_DISP_COLORS))
		return "";
	return cw_strlist_get_str(color_on, level);
}

static const char *get_dbg_color_off(int level)
{
	if (!(cw_dbg_opt_display & DBG_DISP_COLORS))
		return "";
	return cw_strlist_get_str(color_off, level);
}

static const char *get_dbg_prefix(int level)
{
	return cw_strlist_get_str(prefix, level);
}

const char *get_dbg_color_ontext(int level)
{
	if (!(cw_dbg_opt_display & DBG_DISP_COLORS))
		return "";
	return cw_strlist_get_str(color_ontext, level);

}





static void cw_dbg_vlog_line(struct cw_LogWriter * writer, 
		const char * prefix, const char * prefix_color,
		const char *textcolor,const char * format, va_list args)
{
	char fbuf[512];

	if ( writer->colored){
		sprintf(fbuf,"DBG: %s%s %s%s%s",
			prefix_color,prefix,textcolor,format,DBG_CLR_OFF);
	}
	else{
		sprintf(fbuf,"DBG: %s %s",
			prefix,format);
				
	}
	writer->write(LOG_DEBUG,fbuf,args,writer);

}



/**
 * Put a list of missing mandatory message elements to debug output
 */
void cw_dbg_missing_mand(int level, struct conn *conn, int ** ml, int n,
			 int  * a)
{
	/*
//      if (!cw_dbg_is_level(DBG_MSG_ERR) || n == 0)
//              return;
*/


	char buffer[2000];
/*	char *p = buffer; */
	int i;
/*	char *delim = "";*/
	
	if (!cw_dbg_is_level(level) || n == 0)
		return;
	
/*
	// TODO XXXX
*/
	for (i = 0; i < n; i++) {
/*		p += sprintf(p, "%s", delim);
		delim = ", ";
		p += sprintf(p, "%s", cw_strelemp(conn->actions, ml[i]->elem_id));
*/
	}
	cw_dbg(level, "Missing mandatory elements: [%s]", buffer);
}




/**
 * Display a packet on for debugger
 */
void cw_dbg_pkt(int level, struct conn *conn, uint8_t * packet, int len,
		struct sockaddr *from)
{

	int hlen;
	char buf[1024];

	if (!cw_dbg_is_level(level))
		return;
	
	if (level == DBG_PKT_IN)
		cw_format_pkt_hdr(buf, 1,  packet, len, from);
	else
		cw_format_pkt_hdr(buf, 0,  packet, len, from);
		
	hlen = cw_get_hdr_msg_offset(packet);

	if (cw_dbg_is_level(DBG_PKT_DMP)) {
		/*
		char *dmp = cw_dbg_mkdmp_c(packet, len, hlen);
		*/
		char * dmp;
		dmp = cw_format_dump(packet,len,NULL);
		
		cw_dbg(level, "%s%s", buf, dmp);
		free(dmp);
	} else
		cw_dbg(level, "%s", buf);
}


void cw_dbg_dmp(int level, const uint8_t * data, int len, const char *format, ...)
{
	char *dmp,*s,*c;
	struct cw_FormatDumpSettings settings;

	if (!cw_dbg_is_level(level))
		return;

	cw_format_get_dump_defaults(&settings);
	
	settings.dump_prefix="  ";
	settings.newline="\n  ";
	settings.dump_suffix="\n";
		

	dmp = cw_format_dump(data, len, &settings);

	/* wrtie the dump line by line to the log file */
	c=dmp;
	while ((s=strchr(c,'\n'))!=NULL){
		*s=0;
		cw_dbg(level," %s",c);
		c=s+1;
	}

	free(dmp);
}


void cw_dbg_msg(int level, struct conn *conn, uint8_t * packet, int len,
		struct sockaddr *from)
{

	char sock_buf[SOCK_ADDR_BUFSIZE];
	char buf[1024];
	char *s;
	struct cw_MsgData search;
	struct cw_MsgData * message;
	const char * msname;	
	uint8_t *msgptr;
	int msg_id;
	
	if (!cw_dbg_is_level(level))
		return;	
	
	s = buf;

	msgptr = cw_get_hdr_msg_ptr(packet);
	msg_id = cw_get_msg_id(msgptr);

	search.type = msg_id;
	message = mavl_get(conn->msgset->msgdata,&search);

	if (!message)
		msname=cw_strmsg(msg_id);
	else
		msname = message->name;
	
	s += sprintf(s, "%s Message (type=%d) ", msname  , msg_id);
	if (level == DBG_MSG_IN)
		s += sprintf(s, "from %s ", sock_addr2str(from,sock_buf));
	else
		s += sprintf(s, "to %s ", sock_addr2str(from,sock_buf));

	s += sprintf(s, ", Seqnum: %d ElemLen: %d", cw_get_msg_seqnum(msgptr),
		     cw_get_msg_elems_len(msgptr));

	cw_dbg(level, "%s", buf);
}

/*

static int cw_format_vendor(char *dst, uint32_t vendor_id, int elem_id,
			    const uint8_t * elem_data)
{
	uint32_t lw_elem_id;
	switch (vendor_id) {
		case CW_VENDOR_ID_CISCO:
		{
			if (elem_id != CW_CISCO_SPAM_VENDOR_SPECIFIC) {
				return sprintf(dst, "%d - %s", elem_id,
					       cw_cisco_id_to_str(elem_id));
			}


			// dive into LWAPP vendor specific decoding 
			lw_elem_id = lw_get_word(elem_data + 4 + 6);
			return sprintf(dst, "%d/LWAPP Vendor: %d - %s",
				       elem_id,
				       lw_elem_id, lw_cisco_id_to_str(lw_elem_id));



			break;
		}
		default:
		{
			return sprintf(dst, "%d", elem_id);

		}



	}

	return 0;
}
*/



void cw_dbg_version_subelem(int level, const char *context, int subtype, 
		uint32_t vendor_id, const uint8_t * vstr, int len)
{
	char v[2048];
	int n;
	
	if (!cw_dbg_is_level(level))
		return;
	if (!vstr)
		return;
	n = cw_format_version(v, vstr, len);

	sprintf(v + n, ", Vendor Id: %d, %s", vendor_id, cw_strvendor(vendor_id));

	cw_dbg(level, "%s: SubType %d, %s", context, subtype, v);
}


void cw_dbgv(struct cw_LogWriter *writer, int level, const char * format, va_list args)
{
	char fbuf[1024];

	if (writer->colored ){
		sprintf(fbuf, "DBG:%s%s %s%s%s",
			get_dbg_color_on(level),
			get_dbg_prefix(level),
			get_dbg_color_ontext(level), format, get_dbg_color_off(level)
		);
	}
	else{
		sprintf(fbuf, "DBG:%s %s",
			get_dbg_prefix(level), format);
	}

	writer->write(LOG_DEBUG,fbuf,args,&cw_log_console_writer);


}

void cw_dbgv1(struct cw_LogWriter *writer, int level, const char * format, ...){
	va_list args;
	va_start(args, format);
	cw_dbgv(writer,level,format,args);
	va_end(args);	
}



void cw_dbg(int level, const char *format, ...){
	int i;
	va_list args;
	
	if (!(cw_dbg_is_level(level)))
		return;
	
	for (i=0; cw_log_writers[i]; i++){
		va_start(args, format);
		cw_dbg_vlog_line(cw_log_writers[i],
			get_dbg_prefix(level),get_dbg_color_on(level),
			get_dbg_color_ontext(level),
			format, args);
		va_end(args);
	}
}

static void cw_dbg_help(struct cw_LogWriter *writer, const char *format, ...)
{
	va_list args;

/*	if(writer->colored){
		sprintf(fbuf, "DBG:%s%s %s%s%s",
			get_dbg_color_on(level),
			get_dbg_prefix(level),
			get_dbg_color_ontext(level), format, get_dbg_color_off(level));
				
		if (dmp!=NULL){
			sprintf(dbuf,"%s%s%s",
				get_dbg_color_ontext(dmp_level),"%s",
				get_dbg_color_off(dmp_level)
			);
		}
		
			
	}
*/
	va_start(args, format);	
	writer->write(LOG_DEBUG,format,args,writer);
	va_end(args);
}

static void builddmp(char *fbuf, char *format, int level, char *dmp, int dmp_level, int colored ){

	char dbuf[128];
	sprintf(fbuf, "DBG:%s%s %s%s%s",get_dbg_color_on(level),
		get_dbg_prefix(level),get_dbg_color_ontext(level), 
		format, get_dbg_color_off(level));
				
	
	if (dmp!=NULL){
		sprintf(dbuf,"%s%s%s",get_dbg_color_ontext(dmp_level),"%s",
				get_dbg_color_off(dmp_level));
	}
	strcat(fbuf,dbuf);
}

void cw_dbg_elem(int level, struct conn *conn, int msg, 
			struct cw_ElemHandler * handler,
			 const uint8_t * msgbuf, int len)
{
	char vendorname[256];
	char vendor_details[265];
	char *dmp;
	int i;
	
	if (!cw_dbg_is_level(level))
		return;
		
	*vendor_details = 0;

	if (handler->vendor){
		sprintf(vendorname,"Vendor %s",cw_strvendor(handler->vendor));
	}
	else{
		sprintf(vendorname,"");
	}

	cw_dbg(level,"%s %d (%s), len=%d ",vendorname,handler->id,
			handler->name,len);

	if (cw_dbg_is_level(DBG_ELEM_DMP)) {
		/*dmp = cw_format_dump(msgbuf,len,NULL);*/
		
		cw_dbg_dmp(DBG_ELEM_DMP,msgbuf,len,"hallo");
	}

	return;





	dmp = NULL;	
	if (cw_dbg_is_level(DBG_ELEM_DMP)) {
		dmp = cw_format_dump(msgbuf,len,NULL);
	}

	for (i=0; cw_log_writers[i]; i++){
		char fbuf[512];
		
		if (cw_log_writers[i]->colored){
			builddmp(fbuf,"%s %d (%s), len=%d ",level,dmp,DBG_ELEM_DMP,cw_log_writers[i]->colored);
			cw_dbg_help(cw_log_writers[i],fbuf,vendorname,handler->id,handler->name,
				len, dmp);
				
		}
		else{
			if (!dmp){
				cw_dbgv1(cw_log_writers[i],level,"%s %d (%s), len=%d ",vendorname,handler->id,
					handler->name,len);
			}
			else{
				cw_dbgv1(cw_log_writers[i],level,"%s %d (%s), len=%d\n%s",vendorname,handler->id,
					handler->name,len,dmp);

			}
		}

/*			cw_dbg(DBG_ELEM_IN, "%s %d (%s), len=%d%s%s",
			vendorname,
		       handler->id, handler->name, len, get_dbg_color_ontext(DBG_ELEM_DMP), dmp);
*/
/*			cw_dbg(DBG_ELEM_IN, "%s %d (%s), len=%d", 
			vendorname,
			handler->id, handler->name, len);
*/
		
		
		
	}

	free(dmp);

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
