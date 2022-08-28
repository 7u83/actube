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
#include "ansi_colors.h"


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







static struct cw_StrListElem theme0[] = {
	{DBG_PKT_IN, ANSI_YELLOW},
	{DBG_PKT_OUT, ANSI_YELLOW ANSI_ITALIC},
	
	{DBG_PKT_DMP_IN, ANSI_BYELLOW},
	{DBG_PKT_DMP_OUT, ANSI_BYELLOW ANSI_ITALIC},
	
	{DBG_MSG_IN, ANSI_BLUE ANSI_BOLD},
	
	{DBG_MSG_OUT, ANSI_BLUE ANSI_BOLD ANSI_ITALIC},
	{DBG_MSG_COMPOSE, ANSI_BLUE ANSI_ITALIC},
	
	
	{DBG_MSG_DMP_IN, ANSI_BBLUE },
	{DBG_MSG_DMP_OUT, ANSI_BBLUE ANSI_ITALIC},
	
	{DBG_ELEM_IN, ANSI_DEFAULT},
	{DBG_ELEM_OUT, ANSI_DEFAULT ANSI_ITALIC},
	
	{DBG_MSG_ERR, ANSI_RED},
	{DBG_PKT_ERR, ANSI_RED},
	{DBG_ELEM_ERR, ANSI_RED},
	
//	{DBG_SUBELEM, ANSI_BBLACK},
	{DBG_DTLS, ANSI_MAGENTA ANSI_BOLD},
	{DBG_DTLS_DETAIL, ANSI_MAGENTA},
	{DBG_DTLS_BIO, ANSI_BMAGENTA},

	{DBG_INFO, ANSI_DEFAULT},

	{DBG_STATE, ANSI_GREEN ANSI_BOLD},

	{DBG_RFC, ANSI_BRED},
	{DBG_X, "\x1b[31m"},
	{DBG_WARN, ANSI_CYAN},
	{DBG_MOD_DETAIL, ANSI_WHITE},
	{DBG_MOD, ANSI_BBLACK ANSI_BOLD },
//	{DBG_CFG_DMP, ANSI_BCYAN }, 
	{DBG_CFG_UPDATES,ANSI_GREEN},	

	{CW_STR_STOP, ""}
};

static struct cw_StrListElem * color_on = theme0;


struct cw_StrListElem color_ontext[] = {

/*	{DBG_ELEM_DMP, "\x1b[37m"},*/
	{DBG_ELEM_DMP_OUT, ANSI_BBLACK ANSI_ITALIC},
	{DBG_ELEM_DMP_IN,  ANSI_BBLACK},

	{CW_STR_STOP, ""}
};

/*
static struct cw_StrListElem color_off[] = {

	{CW_STR_STOP, "\x1b[22;39m\x1b[23m"}
};
*/

static struct cw_StrListElem prefix[] = {
	{DBG_INFO, 		"Info - "},
	{DBG_PKT_IN, 		"Pkt In  - "},
	{DBG_PKT_OUT, 		"Pkt Out - "},
	{DBG_MSG_IN, 		"Msg In  - "},
	{DBG_MSG_OUT,		"Msg Out - "},

	{DBG_ELEM_IN, 		"  Msg Element - "},
	{DBG_ELEM_OUT,		"  Msg Element - "},
	
	{DBG_MSG_ERR, 		"  Msg Error - "},
	{DBG_PKT_ERR, 		"  Pkt Error - "},
	{DBG_ELEM_ERR, 		"  Elem Error - "},
	{DBG_RFC, 		"  RFC - "},
	{DBG_DTLS, 		"DTLS - "},
	{DBG_DTLS_DETAIL,	 "DTLS - "},
	{DBG_WARN,	 	"  Warning - "},
	{DBG_MOD, 		"Mod - "},
	{DBG_MOD_DETAIL,	"Mod - "},
	{DBG_STATE, 		"STATEMACHINE - "},
	{DBG_CFG_UPDATES,	"Cfg - "},



	{DBG_X, "XXXXX - "},


	{CW_STR_STOP, ""}
};

static const char *get_dbg_color_on(int level)
{
	if (!(cw_dbg_opt_display & DBG_DISP_COLORS))
		return "";
	return cw_strlist_get_str(color_on, level);
}

/*
static const char *get_dbg_color_off(int level)
{
	if (!(cw_dbg_opt_display & DBG_DISP_COLORS))
		return "";
	return cw_strlist_get_str(color_off, level);
}
*/

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


/**
 * Check if a specific debug level is set.
 * @param level Level to check
 * @return 0 if leveln is not set, otherwise level is set
 */

int cw_dbg_is_level(int level)
{
	if (level > 1 && (level &1))
		return 1;

/*	if (level >= DBG_ALL ){
		return 1;
	}*/
	return (cw_dbg_opt_level & (level));
}


static void cw_dbg_vlog_line(struct cw_LogWriter * writer, 
		const char * prefix, const char * prefix_color,
		const char *textcolor,const char * format, va_list args)
{
	char fbuf[512];

	if ( writer->colored){
		sprintf(fbuf,"DBG: %s%s%s%s%s",
			prefix_color,prefix,textcolor,format,DBG_CLR_OFF);
	}
	else{
		sprintf(fbuf,"DBG: %s%s",
			prefix,format);
				
	}
	writer->write(LOG_DEBUG,fbuf,args,writer);

}



/**
 * Put a list of missing mandatory message elements to debug output
 */
void cw_dbg_missing_mand(int level, struct cw_Conn *conn, int ** ml, int n,
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
void cw_dbg_pkt(int level, struct cw_Conn *conn, uint8_t * packet, int len,
		struct sockaddr *from)
{

/*	int hlen;*/
	char buf[1024];

	if (!cw_dbg_is_level(level))
		return;
	
	if (level == DBG_PKT_IN)
		cw_format_pkt_hdr(buf, 1,  packet, len, from);
	else
		cw_format_pkt_hdr(buf, 0,  packet, len, from);
		
/*	hlen = cw_get_hdr_msg_offset(packet);*/

	cw_dbg(level, "%s", buf);
		
//	if (cw_dbg_is_level(DBG_PKT_DMP)) {
		int dlevel;
		if (level == DBG_PKT_IN){
			dlevel=DBG_PKT_DMP_IN;
		}
		else{
			dlevel=DBG_PKT_DMP_OUT;
		}
		cw_dbg_dmp(dlevel,packet,len,"");
//	} 

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


void cw_dbg_msg(int level, struct cw_Conn *conn, uint8_t * packet, int len,
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
	
	
//	if (cw_dbg_is_level(DBG_MSG_DMP)){
		int dlevel;
		if(level==DBG_MSG_IN){
			dlevel = DBG_MSG_DMP_IN;
		}
		else{
			dlevel = DBG_MSG_DMP_OUT;
		}
		cw_dbg_dmp(dlevel,packet,len,"");
//	}
	

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



void cw_dbg_elem(int level, struct cw_Conn *conn, int msg, 
	struct cw_ElemHandler * handler, const uint8_t * msgbuf, int len)
{
	char vendorname[256];
	char vendor_details[265];

	
	if (!cw_dbg_is_level(level))
		return;
		
	*vendor_details = 0;

	if (handler->vendor){
		sprintf(vendorname,"Vendor %s",cw_strvendor(handler->vendor));
	}
	else{
		sprintf(vendorname,"%s","");
	}


	cw_dbg(level,"%s %d (%s), len=%d ",vendorname,handler->id,
			handler->name,len);
	
	if (cw_dbg_is_level(DBG_ELEM_DMP)) {
		if (level == DBG_ELEM_OUT)
			cw_dbg_dmp(DBG_ELEM_DMP_OUT,msgbuf,len,"");
		else 
			cw_dbg_dmp(DBG_ELEM_DMP_IN,msgbuf,len,"");

	}

	return;

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
