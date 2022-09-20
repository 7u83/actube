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

#include "dot11.h"

/**
 *@addtogroup DBG
 *@{
 */

uint32_t cw_dbg_opt_display = DBG_DISP_COLORS;

/**
 * Contains all debuglevels currently set
 */
static struct mavl * cw_dbg_opt_level = NULL;


static int dbg_cmp(const void *a, const void*b)
{
	return (*((int*)a)-*((int*)b));
}


struct cw_DbgStr {
	int level;
	const char *str;
	const char ** str_list;
	const char *descr;
};


static const char * dbg_level_msg[] = {
	"msg_in", "msg_out", NULL
};

static const char * dbg_level_pkt[] = {
	"pkt_in", "pkt_out",  NULL
};

static const char * dbg_level_elem[]={
	"elem_in","elem_out",NULL
};

static const char * dbg_level_elem_detail[] = {
	"elem_detail_in", "elem_detail_out", NULL
};

static const char * dbg_level_elem_dmp[] = {
	"elem_dmp_in", "elem_dmp_out", NULL
};
	
static const char * dbg_level_elem_all[] = {
	"elem", "elem_dmp", "elem_detail",NULL
};

static const char * dbg_level_std[] = {
	"msg","elem","msg_err", "elem_err", "pkt_err", "rfc", "warn", "state", "info", NULL
};


/**
 * Debug strings
 */
struct cw_DbgStr cw_dbg_strings[] = {
	{ 0, 			"std", dbg_level_std, "some useful standard options to debug CAPWAP"},
	{ 0,			"msg", dbg_level_msg, "messages headers" },
	{ 0,			"pkt", dbg_level_pkt, "packet headers" },
	{ 0,			"elem", dbg_level_elem, "message elemenst" },
	{ 0, 			"elem_detail", dbg_level_elem_detail, "details for message elements"},
	{ 0, 			"elem_dmp", dbg_level_elem_dmp,"hexdump element"},
	{ 0, 			"elem_all", dbg_level_elem_all,"all possible elem options"},

	{ DBG_WARN,		"warn", NULL, "warnings" },

	{ DBG_ELEM_ERR, 	"elem_err", NULL, "errors in elements" },
	{ DBG_PKT_ERR,		"pkt_err", NULL, "packets with errors which are usually would be discard" },
	{ DBG_MSG_ERR,		"msg_err", NULL, "message errors"},
	{ DBG_RFC,		"rfc", NULL, "errors related to RFC5415/RFC5416 viloations" },
	{ DBG_CFG_UPDATES, 	"cfg_updates",NULL, "show configuration aupdates" },
	{ DBG_MOD,		"mod",NULL, "module related"},
	{ DBG_MOD_DETAIL,	"mod_detail",NULL, "module related details"},
	{ DBG_STATE,		"state",NULL, "CAWPAP states" },
	{ DBG_MSG_COMPOSE,	"msg_compose",NULL,"message composing details" },


	{ DBG_INFO,		"info", NULL, "several infos are displayed" },
	{ DBG_PKT_IN,		"pkt_in",NULL, "headers of incomming packets."  },
	{ DBG_PKT_OUT,		"pkt_out",NULL, "headers out outgoing packets." },
	{ DBG_PKT_DMP_IN,	"pkt_dmp_in",NULL, "hex-dump incomming packets" },
	{ DBG_PKT_DMP_OUT,	"pkt_dmp_out",NULL, "hex-dump outgoing packets"  },

	{ DBG_MSG_IN,		"msg_in", NULL, "incomming messages" },
	{ DBG_MSG_OUT,		"msg_out", NULL, "outgoing messages" },
	{ DBG_MSG_DMP_IN,	"msg_dmp_in", NULL, "hex-dump incomming messages" },
	{ DBG_MSG_DMP_OUT,	"msg_dmp_out", NULL,"hex-dump of outgong messages"  },
	

	{ DBG_ELEM_IN,  	"elem_in", NULL, "elements of incomming messages"},
	{ DBG_ELEM_OUT, 	"elem_out", NULL, "elements of outgoing messages"},
	{ DBG_ELEM_DETAIL_IN, 	"elem_detail_in", NULL, "details of incomming message elements" },
	{ DBG_ELEM_DETAIL_OUT, 	"elem_detail_out",NULL, "details of outgoing message elememnts" },
	{ DBG_ELEM_DMP_IN, 	"elem_dmp_in", NULL, "hex-dump of incoming elements"},
	{ DBG_ELEM_DMP_OUT, 	"elem_dmp_out", NULL, "hex-dump of outgoing elements"},
	{ DBG_ELEM_VNDR,	"elem_vndr", NULL, "expand vendor specific payloads"},	
	
	{ DBG_DTLS, 		"dtls",NULL, "DTLS related stuff"},
	{ DBG_DTLS_BIO,		"dtls_bio",NULL, "DTLS BIO releated" },
	{ DBG_DTLS_BIO_DMP,	"dtls_bio_dmp",NULL, "hex-dump dtls packets" },
	{ DBG_DTLS_DETAIL, 	"dtls_detail",NULL, "DTLS detailed messages"},

	{ DBG_X,		"x", NULL, "only for developers" },

	{ DBG_ALL, 		"all", NULL, "all debug levels at once"},

	
	{ 0, NULL } 
};



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
	{DBG_WARN,	 	"Warning - "},
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
	if (cw_dbg_opt_level == NULL)
		return 0;

	return mavl_get(cw_dbg_opt_level,&level) == NULL ? 0:1;
}

/**
  * Set debug level
  * @param level debug level to set, allowed values are enumberated in #cw_dbg_levels structure.
  * @param on 1: turns the specified debug level on, 0: turns the specified debug level off.
  *
  * To check if a specific debug level is set, call #cw_dbg_is_level.
  */
void cw_dbg_set_level (int level, int on)
{
	int exists;

	if (cw_dbg_opt_level == NULL){
		cw_dbg_opt_level = mavl_create(dbg_cmp,NULL,sizeof(int));
		if (cw_dbg_opt_level == NULL)
			return;
	}

	if (on){
		mavl_insert(cw_dbg_opt_level,&level,&exists);
	}
	else 
		mavl_del(cw_dbg_opt_level,&level);
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
 * Display a packet on debugger
 */
void cw_dbg_pkt(int level, struct cw_Conn *conn, uint8_t * packet, int len,
		struct sockaddr *from)
{

	char buf[1024];
	int (*fmt_pkt_hdr)(char *dst, int incomming, uint8_t * packet, int len,
		      struct sockaddr *from);
	fmt_pkt_hdr = NULL;
	if (conn){
		if (conn->msgset)
			fmt_pkt_hdr = conn->msgset->format_pkt_hdr;
	}
	if (fmt_pkt_hdr==NULL){
		fmt_pkt_hdr = cw_format_pkt_hdr;
	}


	if (!cw_dbg_is_level(level))
		return;
	
	if (level == DBG_PKT_IN)
		fmt_pkt_hdr(buf, 1,  packet, len, from);
	else
		fmt_pkt_hdr(buf, 0,  packet, len, from);
		
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
	
	if (cw_dbg_is_level(DBG_ELEM_DMP_OUT) || cw_dbg_is_level(DBG_ELEM_DMP_IN)) {
		if (level == DBG_ELEM_OUT)
			cw_dbg_dmp(DBG_ELEM_DMP_OUT,msgbuf,len,"");
		else 
			cw_dbg_dmp(DBG_ELEM_DMP_IN,msgbuf,len,"");

	}

	return;

}




int cw_dbg_set_level_from_str0(const char *level,int on)
{
	int i,all;
	all = strcmp(level,"all")==0 ? 1: 0;
	
	for(i=0; cw_dbg_strings[i].str != NULL; i++){
		if (strcmp(cw_dbg_strings[i].str,level)==0 || all){
			if (cw_dbg_strings[i].str_list==NULL)
				cw_dbg_set_level(cw_dbg_strings[i].level,on);
			else {
				const char **l;
				for (l=cw_dbg_strings[i].str_list; *l; l++){
					if (!cw_dbg_set_level_from_str0(*l,on)){
						stop();
					}
				}
			}
			if (!all)
				return 1;
		}
	}	
	if (all)
		return 1;
/*	blevel = cw_strlist_get_id(cw_dbg_strings, slevel);
	if (blevel==-1)
		return 0;
*/		
//	cw_dbg_set_level(blevel,on);
//	return 1;
	return 0;
}


int cw_dbg_set_level_from_str(const char *level)
{
	int on;
	const char *slevel;
	
	switch(*level){
		case '-':
		case '!':
		case '~':
			on =0;
			slevel=level+1;
			break;
		case '+':
			slevel=level+1;
			on=1;
			break;
		default:
			slevel=level;
			on=1;
	}
	return cw_dbg_set_level_from_str0(slevel,on);
}




void cw_dbg_print_help(FILE *out, const char * prefix)
{
	struct cw_DbgStr *s;
	for (s=cw_dbg_strings; s->str != NULL; s++){
		const char * descr = s->descr!=NULL ? s->descr : "";
		fprintf(out,"%s%s: %s",prefix,s->str,descr);
		if (s->str_list != NULL){
			const char **l;
			char *sp="";
			fprintf(out," (");
			for(l = s->str_list; *l!=NULL; l++){
				fprintf(out,"%s%s",sp,*l);
				sp=" ";
			}
			fprintf(out,")");
		}
		fprintf(out,"\n");
	}
}



void cw_dbg_dot11_elems(const uint8_t *src,int len)
{
	uint8_t id,l;
	int p;
	char str[1024];

	for(p=0; p<len; p+=l+2){
		if (len-p<3){
			cw_dbg(DBG_X,"Error in dot11 element");
			return;
		}
		id=src[p];
		l=src[p+1];
		cw_format_dot11_elem(str,id,src+p+2,l);
		cw_dbg_dmp(DBG_X,src+p+2,l,"");
		cw_dbg(DBG_X,str);
	}

}

void cw_dbg_dot11_frame(uint8_t * frame,int len)
{
	char hdr[1024];
	cw_format_dot11_hdr(hdr, frame, len);
	cw_dbg(DBG_X,"%s",hdr);

	int type =dot11_get_type_and_subtype(frame);

	switch (type){
		case DOT11_ASSOC_REQ:
			cw_dbg_dot11_elems(frame+28,len-28);
			break;
		case DOT11_ASSOC_RESP:
			cw_dbg(DBG_X,"  Capapility info: %04X",dot11_assoc_resp_get_cap(frame)); 
			cw_dbg(DBG_X,"  Status CodeCapapility info: %04X",dot11_assoc_resp_get_status_code(frame));
			cw_dbg(DBG_X,"  Assoc ID: %04X",dot11_assoc_resp_get_assoc_id(frame)); 

			cw_dbg_dot11_elems((frame+DOT11_BODY_POS+DOT11_ASSOC_RESP_BODY_LEN),
					len-DOT11_BODY_POS-DOT11_ASSOC_RESP_BODY_LEN);
			break;
			
	}
}


/**@}*/


