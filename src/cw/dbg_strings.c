/*
    This file is part of actube.

    actube is free software: you can redistribute it and/or modify
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
 *@brief 
 */



#include "dbg.h"

/**
 *@addtogroup DbgOptions 
 *@{
 */


/**
 * Debug strings
 */
struct cw_StrListElem cw_dbg_strings[] = {
	{ DBG_INFO,		"info",  },
	{ DBG_PKT_IN,		"pkt_in"  },
	{ DBG_PKT_OUT,		"pkt_out" },
	{ DBG_PKT_ERR,		"pkt_err" },
	{ DBG_PKT_DMP_IN,	"pkt_dmp_in" },
	{ DBG_PKT_DMP_OUT,	"pkt_dmp_out" },

	{ DBG_MSG_IN,		"msg_in"  },
	{ DBG_MSG_OUT,		"msg_out" },
	{ DBG_MSG_DMP_IN,	"msg_dmp_in"  },
	{ DBG_MSG_DMP_OUT,	"msg_dmp_out" },
	
	{ DBG_MSG_ERR,		"msg_err"},

	{ DBG_RFC,		"rfc", 	 },

	{ DBG_ELEM_IN,  	"elem_in"},
	{ DBG_ELEM_OUT, 	"elem_out"},
	{ DBG_ELEM_DMP, 	"elem_dmp"},
	{ DBG_ELEM_ERR, 	"elem_err" },
	{ DBG_ELEM_DETAIL_IN, 	"elem_detail_in" },
	{ DBG_ELEM_DETAIL_OUT, 	"elem_detail_out" },
	{ DBG_ELEM_VNDR,	"elem_vndr"},	
	
	{ DBG_DTLS, 		"dtls" },
	{ DBG_DTLS_BIO,		"dtls_bio" },
	{ DBG_DTLS_BIO_DMP,	"dtls_bio_dmp"},
	{ DBG_DTLS_DETAIL, 	"dtls_detail"},

	{ DBG_CFG_UPDATES, 	"cfg_updates" },
	{ DBG_X,		"x" },

	
//	{DBG_CFG_DMP, "cfg_dmp" },
	
	{ DBG_WARN,		"warn" },
		
	{ DBG_MOD,		"mod"},
	{ DBG_STATE,		"state" },
	{ DBG_MSG_COMPOSE,	"msg_compose" },

	{ (DBG_MSG_IN | DBG_MSG_OUT), "msg" },
	{ (DBG_PKT_IN | DBG_PKT_OUT), "pkt" },
	{ (DBG_ELEM_IN | DBG_ELEM_OUT), "elem" },
	{ (DBG_ELEM_DETAIL_IN | DBG_ELEM_DETAIL_OUT), "elem_detail" },
	{ (DBG_ELEM_IN | DBG_ELEM_OUT | DBG_ELEM_DMP | DBG_ELEM_DETAIL_IN | DBG_ELEM_DETAIL_OUT), "elem_all" },

	{ (	DBG_MSG_IN | DBG_MSG_OUT | 
		DBG_ELEM_IN | DBG_ELEM_OUT |
		DBG_MSG_ERR | DBG_ELEM_ERR |
		DBG_PKT_ERR | DBG_RFC | DBG_WARN
		| DBG_STATE | DBG_INFO), 			"std" },

	{ DBG_ALL, 		"all"},

	
	{ CW_STR_STOP, NULL } 
};

/**
 *@}
 */




/*
struct cw_dbg_cfgstrs cw_dbg_cfgstrs[] = {

	{"info",DBG_CW_INFO},
	{"msg",DBG_MSG},
	{"elem",DBG_ELEM},
	{"elem_dmp",DBG_ELEM_DMP},
	{"rfc",DBG_CW_RFC},
	{"pkt_in",DBG_CW_PKT_IN},
	{"pkt_out",DBG_CW_PKT_OUT},
	{"pkt_dtl",DBG_CW_PKT_DTL},
	{"pkt",(DBG_CW_PKT_IN | DBG_CW_PKT_OUT)},
	{"pkt_dmp",DBG_CW_PKT_DMP},
	{"pkt_err",DBG_CW_PKT_ERR},
	{"msg_err",DBG_MSG_ERR},
	{"img_dtl",DBG_CW_IMG_DTL},
	{"elem_err",DBG_ELEM_ERR},

	{"dtls",DBG_DTLS},
	{"dtls_detail",DBG_DTLS_DETAIL},
	{"dtls_bio",DBG_DTLS_BIO},
	{"dtls_bio_dmp",DBG_DTLS_BIO_DMP},


	{"all",DBG_ALL},
	{"err",DBG_ERR},
	{0,0}

};
*/


