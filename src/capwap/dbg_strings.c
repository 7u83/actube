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


#include <string.h>

#include "dbg.h"


struct cw_str cw_dbg_strings[] = {
	{ DBG_INFO, "info" },
	{ DBG_PKT_IN,   "pkt_in" },
	{ DBG_PKT_OUT, "pkt_out" },
	{ DBG_MSG_IN, "msg_in" },
	{ DBG_MSG_ERR, "msg_err" },
	{ DBG_ELEM,  "elem" },
	{ DBG_ELEM_DMP, "elem_dmp" },
	{ DBG_ELEM_ERR, "elem_err" },
	{ DBG_X,"dbgx" },
	{ CW_STR_STOP, NULL } 
};





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

/*
int cw_log_str2dbglevel(const char * str)
{
	int i;
	for (i=0; cw_dbg_cfgstrs[i].name; i++) {
		if (!strcmp(str,cw_dbg_cfgstrs[i].name))
			return cw_dbg_cfgstrs[i].level;
	}
	return 0;
}
*/
