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
	{ DBG_INFO,	"info", "Schow diverse info messages" },
	{ DBG_PKT_IN,	"pkt_in" , "Show headers of incomming CAPWAP packets"},
	{ DBG_PKT_OUT,	"pkt_out", "Show headers of outgoing CAPWAP packets" },
	{ DBG_PKT_ERR,	"pkt_err", "Incomming CAPWAP packets with errors, wich would"
				  "usually silently discarded" },
	{ DBG_PKT_DMP,	"pkt_dmp", "Dump content of incomming/outgoing packets." 
				  "Requires pkt_in/pkt_out"},
	{ DBG_MSG_IN,	"msg_in", "Display incomming CAPWAP/LWAPP messages." },
	{ DBG_MSG_OUT,	"msg_out","Display outgoing CAPWAP/LWAPP messages."},
	{ DBG_MSG_ERR,	"msg_err", "Messages wirh errors"  },

	{ DBG_RFC,	"rfc", 	    "RFC related errors"},
	{ DBG_ELEM_IN,  "elem_in", "Elements of incomming messages" },
	{ DBG_ELEM_OUT, "elem_out", "Elements of outgoing messages" },
	{ DBG_ELEM_DMP, "elem_dmp" , "Dump elements" },

	{ DBG_SUBELEM,	"subelem", "Show sub-elements." },
	{ DBG_SUBELEM_DMP, "subelem_dmp", "Dump sub-elements" },
	
	{ DBG_ELEM_DETAIL, "elem_detail"},

	{ DBG_ELEM_ERR, "elem_err" },
	
	{ DBG_DTLS, "dtls" },
	{ DBG_DTLS_BIO, "dtls_bio" },
	{ DBG_DTLS_BIO_DMP, "dtls_bio_dmp"},
	{ DBG_DTLS_DETAIL, "dtls_detail"},
	
	{ DBG_WARN, "warn" },
	
	{ DBG_MOD,"mod"},
	{ DBG_ALL, "all"},
	
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
	{"dtls_dietail",DBG_DTLS_DETAIL},
	{"dtls_bio",DBG_DTLS_BIO},
	{"dtls_bio_dmp",DBG_DTLS_BIO_DMP},


	{"all",DBG_ALL},
	{"err",DBG_ERR},
	{0,0}

};
*/


